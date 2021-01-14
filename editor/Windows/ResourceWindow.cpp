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
#include "TextureBuilder.h"
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
        
        bool isOpenFolder = false;
        bool isOpenCreatePlane = false;
        bool isOpenCreateUnlitMaterial = false;
        bool isOpenCreateRenderTexture = false;
        ImGui::SetNextWindowSize({200, 150});
        if (ImGui::BeginPopup("AddResource")) {
            
            ImGuiTreeNodeFlags node_flags = ImGuiTreeNodeFlags_SpanFullWidth | ImGuiTreeNodeFlags_Leaf;
            if (ImGui::TreeNodeEx("Folder", node_flags, reinterpret_cast<ImTextureID>(IconManager::Instance().GetTexture(IconType::Folder)))) {
                if (ImGui::IsItemClicked()) {
                    isOpenFolder = true;
                }
                ImGui::TreePop();
            }
            ImGui::Separator();
            ImGui::MenuItem("Texture", NULL, false, false);
            if (ImGui::TreeNodeEx("Image Texture", node_flags, reinterpret_cast<ImTextureID>(IconManager::Instance().GetTexture(IconType::Texture)))) {
                if (ImGui::IsItemClicked()) {
                    igfd::ImGuiFileDialog::Instance()->OpenDialog("ImageTextureDlgKey", ICON_IGFD_FOLDER_OPEN "Choose Image", ".png,.jpg,.tga", ".");
                }
                ImGui::TreePop();
            }
            if (ImGui::TreeNodeEx("Render Texture", node_flags, reinterpret_cast<ImTextureID>(IconManager::Instance().GetTexture(IconType::Texture)))) {
                if (ImGui::IsItemClicked()) {
                    isOpenCreateRenderTexture = true;
                }
                ImGui::TreePop();
            }
            ImGui::Separator();
            ImGui::MenuItem("Mesh", NULL, false, false);
            if (ImGui::TreeNodeEx("Plane", node_flags, reinterpret_cast<ImTextureID>(IconManager::Instance().GetTexture(IconType::Mesh)))) {
                if (ImGui::IsItemClicked()) {
                    isOpenCreatePlane = true;
                }
                ImGui::TreePop();
            }
            ImGui::Separator();
            ImGui::MenuItem("Material", NULL, false, false);
            if (ImGui::TreeNodeEx("Unlit", node_flags, reinterpret_cast<ImTextureID>(IconManager::Instance().GetTexture(IconType::Material)))) {
                if (ImGui::IsItemClicked()) {
                    isOpenCreateUnlitMaterial = true;
                }
                ImGui::TreePop();
            }
            ImGui::EndPopup();
        }
        
        if (isOpenFolder) {
            ImGui::OpenPopup("Folder");
        }
        
        ImGui::SetNextWindowSize({200, 80});
        if (ImGui::BeginPopupModal("Folder", NULL, ImGuiWindowFlags_NoResize)) {
            static char folder[128] = "Default";
            ImGui::InputText("Folder", folder, IM_ARRAYSIZE(folder));
            if (ImGui::Button("OK", ImVec2(50, 0))) {
                createFolder(folder);
                ImGui::CloseCurrentPopup();
            }
            ImGui::SetItemDefaultFocus();
            ImGui::SameLine();
            if (ImGui::Button("Cancel", ImVec2(50, 0))) {
                ImGui::CloseCurrentPopup();
            }
            
            ImGui::EndPopup();
        }
        
        if (isOpenCreateRenderTexture) {
            ImGui::OpenPopup("RenderTexture");
        }
        
        ImGui::SetNextWindowSize({200, 200});
        if (ImGui::BeginPopupModal("RenderTexture", NULL, ImGuiWindowFlags_NoResize)) {
            static char name[128] = "RenderTexture";
            ImGui::InputText("Name", name, IM_ARRAYSIZE(name));
            static int width = 512, height = 512;
            ImGui::InputInt("Width", &width);
            ImGui::InputInt("Height", &height);
            if (ImGui::Button("OK", ImVec2(50, 0))) {
                createRenderTexture(name, width, height);
                ImGui::CloseCurrentPopup();
            }
            ImGui::SetItemDefaultFocus();
            ImGui::SameLine();
            if (ImGui::Button("Cancel", ImVec2(50, 0))) {
                ImGui::CloseCurrentPopup();
            }
            
            ImGui::EndPopup();
        }
        
        if (isOpenCreatePlane) {
            ImGui::OpenPopup("Plane");
        }
        
        ImGui::SetNextWindowSize({200, 80});
        if (ImGui::BeginPopupModal("Plane", NULL, ImGuiWindowFlags_NoResize)) {
            static char name[128] = "Plane";
            ImGui::InputText("Name", name, IM_ARRAYSIZE(name));
            if (ImGui::Button("OK", ImVec2(50, 0))) {
                createPlaneMesh(name);
                ImGui::CloseCurrentPopup();
            }
            ImGui::SetItemDefaultFocus();
            ImGui::SameLine();
            if (ImGui::Button("Cancel", ImVec2(50, 0))) {
                ImGui::CloseCurrentPopup();
            }
            
            ImGui::EndPopup();
        }
        
        if (isOpenCreateUnlitMaterial) {
            ImGui::OpenPopup("Unlit");
        }
        
        ImGui::SetNextWindowSize({200, 80});
        if (ImGui::BeginPopupModal("Unlit", NULL, ImGuiWindowFlags_NoResize)) {
            static char name[128] = "Unlit";
            ImGui::InputText("Name", name, IM_ARRAYSIZE(name));
            if (ImGui::Button("OK", ImVec2(50, 0))) {
                createUnlitMaterial(name);
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
                createImageTexture(fileName, filePathName);
            }
            // close
            igfd::ImGuiFileDialog::Instance()->CloseDialog();
        }
        
        ImGui::Separator();
        m_browserWidget.OnGUI();
        
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
            m_propertyWindow->SetPropertys({std::shared_ptr<TextureGUI>(new TextureGUI(resource))});
        }else if (path.extension() == ".mesh") {
            auto resource = Lutra::ResourceManager::Instance().LoadResource<Lutra::Mesh>(path);
            m_propertyWindow->SetPropertys({std::shared_ptr<MeshGUI>(new MeshGUI(resource))});
        }else if (path.extension() == ".mat") {
            auto resource = Lutra::ResourceManager::Instance().LoadResource<Lutra::Material>(path);
            auto materialUI = std::shared_ptr<MaterialGUI>(new MaterialGUI(resource));
            materialUI->SetResourceChangedCallback([path, resource](){
                Lutra::ResourceManager::Instance().SaveResource(path, resource);
            });
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

    void ResourceWindow::createImageTexture(const std::string& name, const std::string& imagePath)
    {
        auto texture = TextureBuilder::Build(imagePath);
        texture->SetName(name);
        Lutra::ResourceManager::Instance().SaveResource(getFolder(m_selectedPath) + "/" + name + ".tex", texture);
    }

    void ResourceWindow::createRenderTexture(const std::string& name, uint32_t width, uint32_t height)
    {
        auto texture = Lutra::ResourceManager::Instance().CreateResource<Lutra::RenderTexture>(width, height, Lutra::TextureFormat::RGBA8, Lutra::TextureFormat::D24S8);
        texture->SetName(name);
        Lutra::ResourceManager::Instance().SaveResource(getFolder(m_selectedPath) + "/" + name + ".rt", texture);
    }

    void ResourceWindow::createPlaneMesh(const std::string& name)
    {
        auto mesh = MeshBuilder::BuildPlane();
        mesh->SetName(name);
        Lutra::ResourceManager::Instance().SaveResource(getFolder(m_selectedPath) + "/" + name + ".mesh", mesh);
    }

    void ResourceWindow::createUnlitMaterial(const std::string& name)
    {
        auto material = MaterialBuilder::BuildUnlint();
        material->SetName(name);
        Lutra::ResourceManager::Instance().SaveResource(getFolder(m_selectedPath) + "/" + name + ".mat", material);
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
    }

}
