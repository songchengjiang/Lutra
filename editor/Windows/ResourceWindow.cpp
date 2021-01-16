//
//  FileBrowserWindow.cpp
//  Editor
//
//  Created by JasonCheng on 2021/1/9.
//

#include "ResourceWindow.h"
#include "PropertyWindow.h"
#include <imguiFileDialog/ImGuiFileDialog.h>
#include <imguiFileDialog/res/CustomFont.h>
#include <imguiFileDialog/res/CustomFont.cpp>
#include <Lutra.h>
#include "ResourceGUI.h"
#include "MeshBuilder.h"
#include "MaterialBuilder.h"

namespace LutraEditor {

    ResourceWindow::ResourceWindow(const std::string& root, PropertyWindow* propertyWindow)
    : m_root(root)
    , m_propertyWindow(propertyWindow)
    , m_browserWidget(root, {{".tex", IconType::Texture}, {".rt", IconType::Texture}, {".mat", IconType::Material}, {".mesh", IconType::Mesh}})
    {
        m_browserWidget.SetClickItemCallback(std::bind(&ResourceWindow::onResourceClicked, this, std::placeholders::_1));
        Lutra::ResourceManifest::Instance().SetRootDirectoty(root);
        m_selectedPath = m_root;
    }

    ResourceWindow::~ResourceWindow()
    {
        
    }

    void ResourceWindow::Open()
    {
        m_isOpen = true;
    }

    void ResourceWindow::Close()
    {
        m_isOpen = false;
    }

    void ResourceWindow::OnGUI(uint32_t width, uint32_t height)
    {
        ImGui::SetNextWindowSize(ImVec2(200, 300), ImGuiCond_FirstUseEver);
        ImGuiWindowFlags window_flags = ImGuiWindowFlags_None;
        ImGui::Begin("Browser", &m_isOpen, window_flags);
        
        if (ImGui::Button(ICON_IGFD_ADD))
            m_isOpenAddResource = true;
        
        if (m_isOpenAddResource) {
            ImGui::OpenPopup("AddResource");
            m_isOpenAddResource = false;
        }
        
        auto treeNode = [](const std::string& name, IconType type) -> bool{
            bool isClicked = false;
            ImGuiTreeNodeFlags node_flags = ImGuiTreeNodeFlags_SpanFullWidth | ImGuiTreeNodeFlags_Leaf;
            if (ImGui::TreeNodeEx(name.c_str(), node_flags, reinterpret_cast<ImTextureID>(IconManager::Instance().GetTexture(type)))) {
                if (ImGui::IsItemClicked()) {
                    isClicked = true;
                }
                ImGui::TreePop();
            }
            return isClicked;
        };
        auto popupWindow = [](const std::string& name) -> std::string{
            std::string result;
            ImGui::SetNextWindowSize({200, 80});
            if (ImGui::BeginPopupModal(name.c_str(), NULL, ImGuiWindowFlags_NoResize)) {
                static char defaultName[128] = "Default";
                ImGui::InputText(name.c_str(), defaultName, IM_ARRAYSIZE(defaultName));
                if (ImGui::Button("OK", ImVec2(50, 0))) {
                    ImGui::CloseCurrentPopup();
                    result = defaultName;
                }
                ImGui::SetItemDefaultFocus();
                ImGui::SameLine();
                if (ImGui::Button("Cancel", ImVec2(50, 0))) {
                    ImGui::CloseCurrentPopup();
                }
                
                ImGui::EndPopup();
            }
            return result;
        };
        bool isOpenFolder = false;
        bool isOpenCreatePlane = false;
        bool isOpenCreateSphere = false;
        bool isOpenCreateUnlitMaterial = false;
        bool isOpenCreateRenderTexture = false;
        ImGui::SetNextWindowSize({200, 150});
        if (ImGui::BeginPopup("AddResource")) {
            
            isOpenFolder = treeNode("Folder", IconType::Folder);
            ImGui::Separator();
            ImGui::MenuItem("Texture", NULL, false, false);
            if (treeNode("Image Texture", IconType::Texture)) {
                igfd::ImGuiFileDialog::Instance()->OpenDialog("ImageTextureDlgKey", ICON_IGFD_FOLDER_OPEN "Choose Image", ".png,.jpg,.tga", ".");
            }
            isOpenCreateRenderTexture = treeNode("Render Texture", IconType::Texture);
            ImGui::Separator();
            ImGui::MenuItem("Mesh", NULL, false, false);
            isOpenCreatePlane = treeNode("Plane", IconType::Mesh);
            isOpenCreateSphere = treeNode("Sphere", IconType::Mesh);
            ImGui::Separator();
            ImGui::MenuItem("Material", NULL, false, false);
            isOpenCreateUnlitMaterial = treeNode("Unlit", IconType::Material);
            ImGui::EndPopup();
        }
        
        if (isOpenFolder) {
            ImGui::OpenPopup("Folder");
        }
        if (isOpenCreatePlane) {
            ImGui::OpenPopup("Plane");
        }
        if (isOpenCreateSphere) {
            ImGui::OpenPopup("Sphere");
        }
        if (isOpenCreateUnlitMaterial) {
            ImGui::OpenPopup("Unlit");
        }
        if (isOpenCreateRenderTexture) {
            ImGui::OpenPopup("RenderTexture");
        }
        
        std::string name;
        if (!(name = popupWindow("Folder")).empty()) {
            createFolder(name);
        }
        
        if (!(name = popupWindow("Plane")).empty()) {
            createResource<PlaneMeshBuilder>(name + std::string(".mesh"), 1, 1);
        }
        
        if (!(name = popupWindow("Sphere")).empty()) {
            createResource<SphereMeshBuilder>(name + std::string(".mesh"), 1.0f, 36, 18);
        }
    
        if (!(name = popupWindow("Unlit")).empty()) {
            createResource<UnlintMaterialBuilder>(name + std::string(".mat"));
        }
        
        ImGui::SetNextWindowSize({200, 200});
        if (ImGui::BeginPopupModal("RenderTexture", NULL, ImGuiWindowFlags_NoResize)) {
            static char name[128] = "RenderTexture";
            ImGui::InputText("Name", name, IM_ARRAYSIZE(name));
            static int width = 512, height = 512;
            ImGui::InputInt("Width", &width);
            ImGui::InputInt("Height", &height);
            if (ImGui::Button("OK", ImVec2(50, 0))) {
                createResource<RenderTextureBuilder>(name + std::string(".rt"), width, height);
                ImGui::CloseCurrentPopup();
            }
            ImGui::SetItemDefaultFocus();
            ImGui::SameLine();
            if (ImGui::Button("Cancel", ImVec2(50, 0))) {
                ImGui::CloseCurrentPopup();
            }
            
            ImGui::EndPopup();
        }
        
        if (igfd::ImGuiFileDialog::Instance()->FileDialog("ImageTextureDlgKey", ImGuiWindowFlags_NoCollapse, {600, 200}, {1200, 800}))
        {
            if (igfd::ImGuiFileDialog::Instance()->IsOk == true)
            {
                std::string filePathName = igfd::ImGuiFileDialog::Instance()->GetFilePathName();
                std::string fileName = filePathName.substr(filePathName.find_last_of("/\\") + 1);
                fileName = fileName.substr(0, fileName.find_last_of('.'));
                createResource<TextureBuilder>(fileName + ".tex", filePathName);
            }
            // close
            igfd::ImGuiFileDialog::Instance()->CloseDialog();
        }
        
        ImGui::Separator();
        if (!m_browserWidget.OnGUI() && ImGui::IsWindowHovered()) {
            if (ImGui::IsMouseClicked(ImGuiMouseButton_Left)) {
                onResourceClicked(m_root);
                m_browserWidget.Reset();
            }
        }
        showMainPopup();
        
        ImGui::End();
    }

    void ResourceWindow::showMainPopup()
    {
        if (m_selectedPath == m_root)
            return;
        if (ImGui::IsWindowHovered(ImGuiHoveredFlags_RootWindow) && ImGui::IsMouseClicked(ImGuiMouseButton_Right))
            ImGui::OpenPopup("MainPopup");
        if (ImGui::BeginPopup("MainPopup")) {
            if (ImGui::MenuItem("Delete")) {
                destroyResource(m_selectedPath);
            }
            if (ImGui::MenuItem("Add")) {
                m_isOpenAddResource = true;
            }
            ImGui::EndPopup();
        }
    }

    void ResourceWindow::onResourceClicked(const std::filesystem::path& path)
    {
        m_propertyWindow->Clear();
        if (path.extension() == ".tex" || path.extension() == ".rt") {
            auto resource = Lutra::ResourceManager::Instance().LoadResource<Lutra::Texture>(path);
            m_propertyWindow->SetPropertys({std::shared_ptr<TextureGUI>(new TextureGUI(resource, [path, resource](){
                Lutra::ResourceManager::Instance().SaveResource(path, resource);
            }))});
        }else if (path.extension() == ".mesh") {
            auto resource = Lutra::ResourceManager::Instance().LoadResource<Lutra::Mesh>(path);
            m_propertyWindow->SetPropertys({std::shared_ptr<MeshGUI>(new MeshGUI(resource, nullptr))});
        }else if (path.extension() == ".mat") {
            auto resource = Lutra::ResourceManager::Instance().LoadResource<Lutra::Material>(path);
            auto materialUI = std::shared_ptr<MaterialGUI>(new MaterialGUI(resource, [path, resource](){
                Lutra::ResourceManager::Instance().SaveResource(path, resource);
            }));
            m_propertyWindow->SetPropertys({materialUI});
        }
        m_selectedPath = path;
    }

    std::string ResourceWindow::getFolder(const std::filesystem::path& path)
    {
        std::string folder = path;
        if (!std::filesystem::is_directory(path)) {
            folder = folder.substr(0, folder.find_last_of("/\\"));
        }
        return folder;
    }

    void ResourceWindow::createFolder(const std::string& folder)
    {
        std::filesystem::create_directory(m_root + "/" + folder);
    }

    void ResourceWindow::destroyResource(const std::filesystem::path& path)
    {
        bool isDirectory = std::filesystem::is_directory(path);
        if (isDirectory) {
            std::filesystem::directory_iterator end_iter;
            for (std::filesystem::directory_iterator iter(path); iter != end_iter; ++iter) {
                destroyResource(*iter);
            }
        }
        if (!isDirectory)
            Lutra::ResourceManager::Instance().DestroyResource(Lutra::ResourceManifest::Instance().FindUUID(path));
        std::filesystem::remove(path);
        Lutra::ResourceManifest::Instance().Save(m_root + "/manifest.yaml");
    }

}
