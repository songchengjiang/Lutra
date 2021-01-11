//
//  SpriteGUI.cpp
//  Editor
//
//  Created by JasonCheng on 2021/1/8.
//

#include "ComponentGUI.h"
#include "IconManager.h"

namespace LutraEditor {

    void TagGUI::OnGUI()
    {
        if (ImGui::CollapsingHeader("Tags", ImGuiTreeNodeFlags_DefaultOpen, reinterpret_cast<ImTextureID>(IconManager::Instance().GetTexture(IconType::Tag)))) {
            auto& tag = m_sceneObject.GetComponent<Lutra::Tag>();
            char name[128] = {0};
            strncpy(name, tag.Name.c_str(), tag.Name.size());
            ImGui::InputText("Name", name, IM_ARRAYSIZE(name));
            tag.Name = name;
         
            const char* items[] = {"Default", "Scene", "PostProcessing"};
            int currentItem = tag.Tags;
            ImGui::Combo("Tag", &currentItem, items, IM_ARRAYSIZE(items));
            tag.Tags = currentItem;
        }
    }

    void TransformGUI::OnGUI()
    {
        if (ImGui::CollapsingHeader("Transform", ImGuiTreeNodeFlags_DefaultOpen, reinterpret_cast<ImTextureID>(IconManager::Instance().GetTexture(IconType::SceneObject)))) {
            auto& transform = m_sceneObject.GetComponent<Lutra::Transform>();
            float* position = glm::value_ptr(transform.Position);
            ImGui::DragFloat3("Position", position);
            transform.Position.x = position[0];
            transform.Position.y = position[1];
            transform.Position.z = position[2];
            
            float* rotation = glm::value_ptr(transform.Rotation);
            ImGui::DragFloat3("Rotation", rotation);
            transform.Rotation.x = rotation[0];
            transform.Rotation.y = rotation[1];
            transform.Rotation.z = rotation[2];
            
            float* scale = glm::value_ptr(transform.Scale);
            ImGui::DragFloat3("Scale", scale);
            transform.Scale.x = scale[0];
            transform.Scale.y = scale[1];
            transform.Scale.z = scale[2];
        }
    }

    void SpriteRendererGUI::OnGUI()
    {
        if (ImGui::CollapsingHeader("SpriteRenderer", ImGuiTreeNodeFlags_DefaultOpen, reinterpret_cast<ImTextureID>(IconManager::Instance().GetTexture(IconType::Square)))) {
            auto& sr = m_sceneObject.GetComponent<Lutra::SpriteRenderer>();
            
            float* size = glm::value_ptr(sr.Size);
            ImGui::DragFloat2("Size", size);
            sr.Size.x = size[0]; sr.Size.y = size[1];
            
            float* anchor = glm::value_ptr(sr.Anchor);
            ImGui::DragFloat2("Anchor", anchor);
            sr.Anchor.x = anchor[0]; sr.Anchor.y = anchor[1];
        }
    }

    void MeshFilterGUI::OnGUI()
    {
        
    }

    void CameraGUI::OnGUI()
    {
        if (ImGui::CollapsingHeader("Camera", ImGuiTreeNodeFlags_DefaultOpen, reinterpret_cast<ImTextureID>(IconManager::Instance().GetTexture(IconType::Camera)))) {
            auto& sr = m_sceneObject.GetComponent<Lutra::Camera>();
            
            ImGui::DragFloat("Fov", &sr.Fov);
            ImGui::DragFloat("AspectRadio", &sr.AspectRadio);
            ImGui::DragFloat("OrthographicSize", &sr.OrthographicSize);
            ImGui::DragFloat("ZNear", &sr.ZNear);
            ImGui::DragFloat("ZFar", &sr.ZFar);

            ImGui::DragFloat4("Viewport", &sr.Viewport_.X);
        }
    }

    void ComponentAppendGUI::OnGUI()
    {
        ImGui::Separator();
        if (ImGui::Button("+ Add Component"))
            ImGui::OpenPopup("AddComponentPopup");
        
        bool needUpdate = false;
        if (ImGui::BeginPopup("AddComponentPopup")) {
            if (ImGui::MenuItem("Camera")) {
                if (!m_sceneObject.HasComponent<Lutra::Camera>()) {
                    m_sceneObject.AddComponent<Lutra::Camera>();
                    needUpdate = true;
                }
            }
            if (ImGui::MenuItem("SpriteRenderer")) {
                if (!m_sceneObject.HasComponent<Lutra::SpriteRenderer>()) {
                    m_sceneObject.AddComponent<Lutra::SpriteRenderer>();
                    needUpdate = true;
                }
            }
                
            ImGui::EndPopup();
        }
        
        if (needUpdate) {
            m_callback();
        }
    }
}
