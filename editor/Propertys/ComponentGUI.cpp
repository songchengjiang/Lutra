//
//  SpriteGUI.cpp
//  Editor
//
//  Created by JasonCheng on 2021/1/8.
//

#include "ComponentGUI.h"
#include "IconManager.h"
#include <imguiFileDialog/ImGuiFileDialog.h>
#include "ResourceBrowserWidget.h"

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
            ImGui::DragFloat3("Position", position, 0.1f);
            transform.Position.x = position[0];
            transform.Position.y = position[1];
            transform.Position.z = position[2];
            
            float* rotation = glm::value_ptr(transform.Rotation);
            ImGui::DragFloat3("Rotation", rotation, 0.1f);
            transform.Rotation.x = rotation[0];
            transform.Rotation.y = rotation[1];
            transform.Rotation.z = rotation[2];
            
            float* scale = glm::value_ptr(transform.Scale);
            ImGui::DragFloat3("Scale", scale, 0.1f);
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
            ImGui::DragFloat2("Size", size, 0.1f);
            sr.Size.x = size[0]; sr.Size.y = size[1];
            
            float* anchor = glm::value_ptr(sr.Anchor);
            ImGui::DragFloat2("Anchor", anchor, 0.1f);
            sr.Anchor.x = anchor[0]; sr.Anchor.y = anchor[1];
            
            int count = (int)sr.Materials.size();
            ImGui::InputInt("MaterialCount", &count);
            count = std::max(0, count);
            sr.Materials.resize(count);
            for (size_t i = 0; i < sr.Materials.size(); ++i) {
                auto& material = sr.Materials[i];
                std::string name = material != nullptr? material->GetName(): "None";
                char str[128] = {0};
                strncpy(str, name.c_str(), name.size());
                ImGui::InputText(("Material " + std::to_string(i)).c_str(), str, IM_ARRAYSIZE(str), ImGuiInputTextFlags_ReadOnly);
                if (ImGui::IsItemClicked()) {
                    ImGui::OpenPopup("MaterialBrowser");
                    m_selectedMaterial = (int)i;
                    m_browserWidget = std::shared_ptr<ResourceBrowserWidget>(new ResourceBrowserWidget(Lutra::ResourceManifest::Instance().GetRootDirectoty(), {{".mat", IconType::Material}}, false));
                }
            }
            
            ImGui::SetNextWindowSize({300, 200});
            if (ImGui::BeginPopupModal("MaterialBrowser", NULL, ImGuiWindowFlags_NoResize)) {
                m_browserWidget->OnGUI();
                if (ImGui::Button("OK", ImVec2(50, 0))) {
                    sr.Materials[m_selectedMaterial] = Lutra::ResourceManager::Instance().LoadResource<Lutra::Material>(m_browserWidget->GetSelectedPath());
                    ImGui::CloseCurrentPopup();
                }
                ImGui::SetItemDefaultFocus();
                ImGui::SameLine();
                if (ImGui::Button("Cancel", ImVec2(50, 0))) {
                    ImGui::CloseCurrentPopup();
                }
                
                ImGui::EndPopup();
            }
        }
    }

    void MeshFilterGUI::OnGUI()
    {
        if (ImGui::CollapsingHeader("MeshFilter", ImGuiTreeNodeFlags_DefaultOpen, reinterpret_cast<ImTextureID>(IconManager::Instance().GetTexture(IconType::Mesh)))) {
            auto& mf = m_sceneObject.GetComponent<Lutra::MeshFilter>();
            
            std::string name = mf.MeshPtr != nullptr? mf.MeshPtr->GetName(): "None";
            char str[128] = {0};
            strncpy(str, name.c_str(), name.size());
            ImGui::InputText("Mesh", str, IM_ARRAYSIZE(str), ImGuiInputTextFlags_ReadOnly);
            if (ImGui::IsItemClicked()) {
                ImGui::OpenPopup("MeshBrowser");
                m_browserWidget = std::shared_ptr<ResourceBrowserWidget>(new ResourceBrowserWidget(Lutra::ResourceManifest::Instance().GetRootDirectoty(), {{".mesh", IconType::Mesh}}, false));
            }
            
            ImGui::SetNextWindowSize({300, 200});
            if (ImGui::BeginPopupModal("MeshBrowser", NULL, ImGuiWindowFlags_NoResize)) {
                m_browserWidget->OnGUI();
                if (ImGui::Button("OK", ImVec2(50, 0))) {
                    mf.MeshPtr = Lutra::ResourceManager::Instance().LoadResource<Lutra::Mesh>(m_browserWidget->GetSelectedPath());
                    ImGui::CloseCurrentPopup();
                }
                ImGui::SetItemDefaultFocus();
                ImGui::SameLine();
                if (ImGui::Button("Cancel", ImVec2(50, 0))) {
                    ImGui::CloseCurrentPopup();
                }
                
                ImGui::EndPopup();
            }
        }
    }

    void CameraGUI::OnGUI()
    {
        if (ImGui::CollapsingHeader("Camera", ImGuiTreeNodeFlags_DefaultOpen, reinterpret_cast<ImTextureID>(IconManager::Instance().GetTexture(IconType::Camera)))) {
            auto& camera = m_sceneObject.GetComponent<Lutra::Camera>();
            
            const char* items[] = {"Perspective", "Orthographic"};
            int currentItem = (int)camera.ProjType;
            ImGui::Combo("ProjectionType", &currentItem, items, IM_ARRAYSIZE(items));
            camera.ProjType = (Lutra::Camera::ProjectionType)currentItem;
            
            ImGui::DragFloat("Fov", &camera.Fov);
            //ImGui::DragFloat("AspectRadio", &camera.AspectRadio);
            ImGui::DragFloat("OrthographicSize", &camera.OrthographicSize);
            ImGui::DragFloat("ZNear", &camera.ZNear, 0.1f);
            ImGui::DragFloat("ZFar", &camera.ZFar, 0.1f);

            if (ImGui::DragFloat4("Viewport", &camera.Viewport_.X, 0.1f)) {
                camera.AspectRadio = ((float)camera.RenderTexture_->GetWidth() * (camera.Viewport_.Width - camera.Viewport_.X)) / (camera.RenderTexture_->GetHeight() * (camera.Viewport_.Height - camera.Viewport_.Y));
            }
            
            std::string name = camera.RenderTexture_ != nullptr? camera.RenderTexture_->GetName(): "None";
            char str[128] = {0};
            strncpy(str, name.c_str(), name.size());
            ImGui::InputText("RenderTexture", str, IM_ARRAYSIZE(str), ImGuiInputTextFlags_ReadOnly);
            if (ImGui::IsItemClicked()) {
                ImGui::OpenPopup("RenderTextureBrowser");
                m_browserWidget = std::shared_ptr<ResourceBrowserWidget>(new ResourceBrowserWidget(Lutra::ResourceManifest::Instance().GetRootDirectoty(), {{".rt", IconType::Texture}}, false));
            }
            
            ImGui::SetNextWindowSize({300, 300});
            if (ImGui::BeginPopupModal("RenderTextureBrowser", NULL, ImGuiWindowFlags_NoResize)) {
                m_browserWidget->OnGUI();
                if (ImGui::Button("OK", ImVec2(50, 0))) {
                    camera.RenderTexture_ = Lutra::ResourceManager::Instance().LoadResource<Lutra::RenderTexture>(m_browserWidget->GetSelectedPath());
                    ImGui::CloseCurrentPopup();
                }
                ImGui::SetItemDefaultFocus();
                ImGui::SameLine();
                if (ImGui::Button("Cancel", ImVec2(50, 0))) {
                    ImGui::CloseCurrentPopup();
                }
                
                ImGui::EndPopup();
            }
        }
    }

    void ComponentAppendGUI::OnGUI()
    {
        ImGui::Separator();
        if (ImGui::Button(ICON_IGFD_ADD " Add Component"))
            ImGui::OpenPopup("AddComponentPopup");
        
        bool needUpdate = false;
        ImGui::SetNextWindowSize({200, 150});
        if (ImGui::BeginPopup("AddComponentPopup")) {
            
            ImGuiTreeNodeFlags node_flags = ImGuiTreeNodeFlags_SpanFullWidth | ImGuiTreeNodeFlags_Leaf;
            ImGui::Separator();
            ImGui::MenuItem("General");
            if (ImGui::TreeNodeEx("Camera", node_flags, reinterpret_cast<ImTextureID>(IconManager::Instance().GetTexture(IconType::Camera)))) {
                if (ImGui::IsItemClicked()) {
                    if (!m_sceneObject.HasComponent<Lutra::Camera>()) {
                        m_sceneObject.AddComponent<Lutra::Camera>();
                        needUpdate = true;
                    }
                    ImGui::CloseCurrentPopup();
                }
                ImGui::TreePop();
            }
            
            ImGui::Separator();
            ImGui::MenuItem("2D", NULL, false, false);
            if (ImGui::TreeNodeEx("SpriteRenderer", node_flags, reinterpret_cast<ImTextureID>(IconManager::Instance().GetTexture(IconType::Square)))) {
                if (ImGui::IsItemClicked()) {
                    if (!m_sceneObject.HasComponent<Lutra::SpriteRenderer>()) {
                        m_sceneObject.AddComponent<Lutra::SpriteRenderer>();
                        needUpdate = true;
                    }
                    ImGui::CloseCurrentPopup();
                }
                ImGui::TreePop();
            }
            if (ImGui::TreeNodeEx("MeshFilter", node_flags, reinterpret_cast<ImTextureID>(IconManager::Instance().GetTexture(IconType::Mesh)))) {
                if (ImGui::IsItemClicked()) {
                    if (!m_sceneObject.HasComponent<Lutra::MeshFilter>()) {
                        m_sceneObject.AddComponent<Lutra::MeshFilter>();
                        needUpdate = true;
                    }
                    ImGui::CloseCurrentPopup();
                }
                ImGui::TreePop();
            }
                
            ImGui::EndPopup();
        }
        
        if (needUpdate) {
            m_callback();
        }
    }
}
