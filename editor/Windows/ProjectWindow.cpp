//
//  ProjectWindow.cpp
//  Editor
//
//  Created by JasonCheng on 2021/1/8.
//

#include "ProjectWindow.h"
#include "PropertyWindow.h"
#include "IconManager.h"

namespace LutraEditor {

    ProjectWindow::ProjectWindow(const std::shared_ptr<Lutra::Scene>& scene, PropertyWindow* propertyWindow)
    : m_scene(scene)
    , m_propertyWindow(propertyWindow)
    {
        m_rootSceneObject = m_scene->CreateSceneObject("Root");
        m_selectedSceneObject = m_rootSceneObject;
    }

    ProjectWindow::~ProjectWindow()
    {
        
    }

    void ProjectWindow::Open()
    {
        m_isOpen = true;
    }

    void ProjectWindow::Close()
    {
        m_isOpen = false;
    }

    void ProjectWindow::OnGUI(uint32_t width, uint32_t height)
    {
        //ImGuiID dockspace_id = ImGui::GetID("DockSpace");
    
        ImGui::SetNextWindowSize(ImVec2(200, 600), ImGuiCond_FirstUseEver);
        //ImGui::SetNextWindowDockID(dockspace_id);
        ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar;
        ImGui::Begin("Project", &m_isOpen, window_flags);
        if (ImGui::IsItemClicked(ImGuiMouseButton_Right))
            m_selectedSceneObject = m_rootSceneObject;
        
        if (m_scene != nullptr) {
            showMainPopup();
            showTree();
        }
        
        ImGui::End();
        
        
    }

    void ProjectWindow::showTree()
    {
        auto& sod = m_rootSceneObject.GetComponent<Lutra::SceneObjectDelegate>();
        for (auto& child : sod.GetChildren()) {
            showTreeNode(child);
        }
    }

    std::vector<std::shared_ptr<PropertyGUI>> ProjectWindow::getComponentGUIs(Lutra::SceneObject so)
    {
        std::vector<std::shared_ptr<PropertyGUI>> guis;
        if (so.HasComponent<Lutra::Tag>()) {
            guis.emplace_back(new TagGUI(so));
        }
        if (so.HasComponent<Lutra::Transform>()) {
            guis.emplace_back(new TransformGUI(so));
        }
        if (so.HasComponent<Lutra::Camera>()) {
            guis.emplace_back(new CameraGUI(so));
        }
        if (so.HasComponent<Lutra::SpriteRenderer>()) {
            guis.emplace_back(new SpriteRendererGUI(so));
        }
        
        guis.emplace_back(new ComponentAppendGUI(so, [this, so](){
            m_propertyWindow->Clear();
            m_propertyWindow->SetPropertys(getComponentGUIs(so));
        }));
        
        return guis;
    }

    void ProjectWindow::showTreeNode(Lutra::SceneObject so)
    {
        ImGuiTreeNodeFlags node_flags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_SpanFullWidth;
        auto& sod = so.GetComponent<Lutra::SceneObjectDelegate>();
        if (sod.GetChildren().empty())
            node_flags |= ImGuiTreeNodeFlags_Leaf;
        if (m_selectedSceneObject == so)
            node_flags |= ImGuiTreeNodeFlags_Selected;
        auto& tag = so.GetComponent<Lutra::Tag>();
        
        IconType type = IconType::SceneObject;
        if (so.HasComponent<Lutra::Camera>()) {
            type = IconType::Camera;
        }
        
        if (ImGui::TreeNodeEx(tag.Name.c_str(), node_flags, reinterpret_cast<ImTextureID>(IconManager::Instance().GetTexture(type)))) {
            if (ImGui::IsItemClicked()) {
                m_propertyWindow->Clear();
                m_propertyWindow->SetPropertys(getComponentGUIs(so));
                m_selectedSceneObject = so;
            }

            for (auto& child : sod.GetChildren()) {
                showTreeNode(child);
            }
            ImGui::TreePop();
        }
    }
    
    void ProjectWindow::showMainPopup()
    {
        if (ImGui::IsWindowHovered(ImGuiHoveredFlags_RootWindow) && ImGui::IsMouseClicked(ImGuiMouseButton_Right))
            ImGui::OpenPopup("MainPopup");
        if (ImGui::BeginPopup("MainPopup")) {
            if (m_selectedSceneObject != m_rootSceneObject)
                if (ImGui::MenuItem("Delete")) {
                    auto parent = m_selectedSceneObject.GetComponent<Lutra::SceneObjectDelegate>().GetParent();
                    destroySceneObject(m_selectedSceneObject);
                    m_propertyWindow->Clear();
                    m_selectedSceneObject = parent? parent: m_rootSceneObject;
                    if (m_selectedSceneObject != m_rootSceneObject) {
                        m_propertyWindow->SetPropertys(getComponentGUIs(m_selectedSceneObject));
                    }
                }
            if (ImGui::MenuItem("Create Scene Object"))
                createSceneObject("Scene Object");
            if (ImGui::MenuItem("Create Camera"))
                createCamera("Camera");
            if (ImGui::BeginMenu("2D")) {
                if (ImGui::MenuItem("Create Sprite"))
                    createSprite("Sprite");
                ImGui::EndMenu();
            }
            ImGui::EndPopup();
        }
    }

    Lutra::SceneObject ProjectWindow::createSceneObject(const std::string& name)
    {
        auto so = m_scene->CreateSceneObject(name);
        m_selectedSceneObject.GetComponent<Lutra::SceneObjectDelegate>().AddChild(so);
        return so;
    }

    Lutra::SceneObject ProjectWindow::createSprite(const std::string& name)
    {
        auto so = createSceneObject("Sprite");
        so.AddComponent<Lutra::SpriteRenderer>();
        return so;
    }

    Lutra::SceneObject ProjectWindow::createCamera(const std::string& name)
    {
        auto so = createSceneObject("Camera");
        so.AddComponent<Lutra::Camera>();
        return so;
    }

    void ProjectWindow::destroySceneObject(Lutra::SceneObject so)
    {
        auto& sod = so.GetComponent<Lutra::SceneObjectDelegate>();
        auto children = sod.GetChildren();
        for (auto& child : children) {
            destroySceneObject(child);
        }
        
        auto parent = sod.GetParent();
        if (parent) {
            parent.GetComponent<Lutra::SceneObjectDelegate>().RemoveChild(so);
        }
        m_scene->DestroySceneObject(so);
    }

}
