//
//  GameWindow.cpp
//  Editor
//
//  Created by JasonCheng on 2021/1/13.
//

#include "GameWindow.h"
#include "Resources/ResourceID.h"

namespace LutraEditor {

    GameWindow::GameWindow(const std::shared_ptr<Lutra::Scene>& scene)
    : m_scene(scene)
    , m_width(1024)
    , m_height(1024)
    {
    }

    GameWindow::~GameWindow()
    {
        
    }

    void GameWindow::Open()
    {
        m_isOpen = true;
    }

    void GameWindow::Close()
    {
        m_isOpen = false;
    }

    void GameWindow::resize(uint32_t width, uint32_t height)
    {
        m_width = width;
        m_height = height;
    }

    void GameWindow::OnGUI(uint32_t width, uint32_t height)
    {
        ImGui::SetNextWindowSize(ImVec2(800, 600), ImGuiCond_FirstUseEver);
        ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoCollapse;
        ImGui::Begin("Game", &m_isOpen, window_flags);
        auto size = ImGui::GetContentRegionAvail();
        if (size.x != m_width || size.y != m_height) {
            resize(size.x, size.y);
        }
        
        std::shared_ptr<Lutra::Texture> deviceTexture;
        for (auto& so : m_scene->GetSceneObjects()) {
            if (so.HasComponent<Lutra::Serializable>()) {
                if (so.HasComponent<Lutra::Camera>()) {
                    auto& camera = so.GetComponent<Lutra::Camera>();
                    if (camera.RenderTexture_ != nullptr) {
                        if (camera.RenderTexture_->GetName() == DEVICE_TEXTURE_RESOURCE_ID) {
                            camera.AspectRadio = ((float)m_width * (camera.Viewport_.Width - camera.Viewport_.X)) / (m_height * (camera.Viewport_.Height - camera.Viewport_.Y));
                            deviceTexture = camera.RenderTexture_;
                        }
                    }
                }
            }
        }
        if (deviceTexture != nullptr)
            ImGui::Image(reinterpret_cast<ImTextureID>(deviceTexture->GetTextureID()), size);
        ImGui::End();
        
    }
}
