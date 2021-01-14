//
//  ResourceGUI.cpp
//  Editor
//
//  Created by JasonCheng on 2021/1/12.
//

#include "ResourceGUI.h"
#include "IconManager.h"
#include "TextureManager.h"
#include "ResourceBrowserWidget.h"

namespace LutraEditor {

    std::string FormatToString(Lutra::TextureFormat format)
    {
        switch (format) {
            case Lutra::TextureFormat::RGB8:
                return "RGB8";
                break;
            case Lutra::TextureFormat::RGBA8:
                return "RGBA8";
                break;
            case Lutra::TextureFormat::D16:
                return "D16";
                break;
            case Lutra::TextureFormat::D24:
                return "D24";
                break;
            case Lutra::TextureFormat::D32F:
                return "D32F";
                break;
            case Lutra::TextureFormat::D24S8:
                return "D24S8";
                break;
            case Lutra::TextureFormat::D32FS8:
                return "D32FS8";
                break;
                
            default:
                break;
        }
    }


    TextureGUI::TextureGUI(const std::shared_ptr<Lutra::Texture>& tex)
    : m_texture(tex)
    {
        if (m_texture->GetType() == Lutra::TextureType::TEX2D) {
            auto tex2D = static_cast<Lutra::Texture2D*>(m_texture.get());
            m_texID = TextureManager::Instance().CreateTextue(tex2D->GetWidth(), tex2D->GetHeight(), tex2D->GetFormat(), tex2D->GetData().data());
        }
    }

    void TextureGUI::OnGUI()
    {
        auto inputText = [](const std::string& name, const std::string& str){
            char buf[128] = {0};
            strncpy(buf, str.c_str(), str.size());
            ImGui::InputText(name.c_str(), buf, IM_ARRAYSIZE(buf), ImGuiInputTextFlags_ReadOnly);
        };
        inputText("Width", std::to_string(m_texture->GetWidth()));
        inputText("Height", std::to_string(m_texture->GetHeight()));
        
        if (m_texture->GetType() == Lutra::TextureType::TEX2D) {
            auto tex2D = static_cast<Lutra::Texture2D*>(m_texture.get());
            inputText("Format", FormatToString(tex2D->GetFormat()));
            ImGui::Separator();
            ImGui::Image(reinterpret_cast<ImTextureID>(m_texID), {ImGui::GetContentRegionAvailWidth(), ImGui::GetContentRegionAvailWidth()});
        }else {
            auto renderTex = static_cast<Lutra::RenderTexture*>(m_texture.get());
            inputText("ColorFormat", FormatToString(renderTex->GetColorFormat()));
            inputText("DepthFormat", FormatToString(renderTex->GetDepthFormat()));
            glm::vec4 clearColor = renderTex->GetClearColor();
            ImGui::ColorEdit4("ClearColor", &clearColor.x);
            renderTex->SetClearColor(clearColor);
        }
    }

    void MeshGUI::OnGUI()
    {
        auto inputText = [](const std::string& name, const std::string& str){
            char buf[128] = {0};
            strncpy(buf, str.c_str(), str.size());
            ImGui::InputText(name.c_str(), buf, IM_ARRAYSIZE(buf), ImGuiInputTextFlags_ReadOnly);
        };
        
        inputText("Vertices", std::to_string(m_mesh->Vertices.size()));
        inputText("Normals", std::to_string(m_mesh->Normals.size()));
        inputText("Tangents", std::to_string(m_mesh->Tangents.size()));
        inputText("Texcoord0", std::to_string(m_mesh->Texcoord0.size()));
        inputText("Texcoord1", std::to_string(m_mesh->Texcoord1.size()));
        uint32_t size = 0;
        for (auto& subMesh : m_mesh->SubMeshList) {
            size += subMesh.Indices.size() / 3;
        }
        inputText("Triangles", std::to_string(size));
    }

    void MaterialGUI::OnGUI()
    {
        bool isMaterialChanged = false;
        for (size_t i = 0; i < m_material->GetPassCount(); ++i) {
            if (ImGui::CollapsingHeader(("Pass " + std::to_string(i)).c_str(), ImGuiTreeNodeFlags_DefaultOpen)) {
                auto& pass = m_material->GetPass(i);
                
                isMaterialChanged |= ImGui::Checkbox("DepthTest", &pass->GetDepthTest());
                isMaterialChanged |= ImGui::Checkbox("DepthWrite", &pass->GetDepthWrite());
                isMaterialChanged |= ImGui::Checkbox("TwoSided", &pass->GetTwoSided());
                glm::bvec4& colorMask = pass->GetColorMask();
                isMaterialChanged |= ImGui::Checkbox("R", &colorMask.r);
                ImGui::SameLine();
                isMaterialChanged |= ImGui::Checkbox("G", &colorMask.g);
                ImGui::SameLine();
                isMaterialChanged |= ImGui::Checkbox("B", &colorMask.b);
                ImGui::SameLine();
                isMaterialChanged |= ImGui::Checkbox("A", &colorMask.a);
                
                const char *blendModes[] = {"Disabled", "Normal", "Screen", "PremultipliedAlpha", "Add", "Multiply"};
                int currentItem = (int)pass->GetBlendMode();
                isMaterialChanged |= ImGui::Combo("BlendMode", &currentItem, blendModes, IM_ARRAYSIZE(blendModes));
                pass->GetBlendMode() = (Lutra::BlendMode)currentItem;
                
                const char *cullFaces[] = {"Front", "Back", "ScFrontAndBackreen"};
                currentItem = (int)pass->GetCullMode();
                isMaterialChanged |= ImGui::Combo("CullMode", &currentItem, cullFaces, IM_ARRAYSIZE(cullFaces));
                pass->GetCullMode() = (Lutra::CullMode)currentItem;
                
                ImGui::Separator();
                
                for (auto& uniform : pass->GetShader().GetValues()) {
                    switch (uniform.second.Type_) {
                        case Lutra::ShaderValue::Float:
                        {
                            float value = uniform.second.Value_.v1;
                            isMaterialChanged |= ImGui::SliderFloat(uniform.first.c_str(), &value, 0.0f, 1.0f);
                            pass->GetShader().SetFloat(uniform.first, value);
                        }
                            break;
                        case Lutra::ShaderValue::Float2:
                        {
                            glm::vec2 value = uniform.second.Value_.v2;
                            isMaterialChanged |= ImGui::SliderFloat2(uniform.first.c_str(), &value.x, 0.0f, 1.0f);
                            pass->GetShader().SetFloat2(uniform.first, value);
                        }
                            break;
                        case Lutra::ShaderValue::Float3:
                        {
                            glm::vec3 value = uniform.second.Value_.v3;
                            isMaterialChanged |= ImGui::SliderFloat3(uniform.first.c_str(), &value.x, 0.0f, 1.0f);
                            pass->GetShader().SetFloat3(uniform.first, value);
                        }
                            break;
                        case Lutra::ShaderValue::Float4:
                        {
                            glm::vec4 value = uniform.second.Value_.v4;
                            isMaterialChanged |= ImGui::ColorEdit4(uniform.first.c_str(), &value.x);
                            pass->GetShader().SetFloat4(uniform.first, value);
                        }
                            break;
                        case Lutra::ShaderValue::Sampler:
                        {
                            auto& sampler = *uniform.second.Value_.tex;
                            std::string name = sampler != nullptr? sampler->GetName(): "None";
                            char str[128] = {0};
                            strncpy(str, name.c_str(), name.size());
                            ImGui::InputText(uniform.first.c_str(), str, IM_ARRAYSIZE(str), ImGuiInputTextFlags_ReadOnly);
                            if (ImGui::IsItemClicked()) {
                                ImGui::OpenPopup("TextureBrowser");
                                m_browserWidget = std::shared_ptr<ResourceBrowserWidget>(new ResourceBrowserWidget(Lutra::ResourceManifest::Instance().GetRootDirectoty(), {{".tex", IconType::Texture}, {".rt", IconType::Texture}}, false));
                            }
                            
                            ImGui::SetNextWindowSize({300, 200});
                            if (ImGui::BeginPopupModal("TextureBrowser", NULL, ImGuiWindowFlags_NoResize)) {
                                m_browserWidget->OnGUI();
                                if (ImGui::Button("OK", ImVec2(50, 0))) {
                                    pass->GetShader().SetSampler(uniform.first, Lutra::ResourceManager::Instance().LoadResource<Lutra::Texture>(m_browserWidget->GetSelectedPath()));
                                    isMaterialChanged |= true;
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
                            break;
                            
                        default:
                            break;
                    }
                }
            }
        }
        if (isMaterialChanged && m_changedCallback != nullptr) {
            m_changedCallback();
        }
    }
}
