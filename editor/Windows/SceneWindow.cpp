//
//  SceneWindow.cpp
//  Editor
//
//  Created by JasonCheng on 2021/1/8.
//

#include "SceneWindow.h"

namespace LutraEditor {

    SceneWindow::SceneWindow(const std::shared_ptr<Lutra::Scene>& scene)
    : m_scene(scene)
    , m_width(1280)
    , m_height(720)
    {
        m_sceneCamera = m_scene->CreateSceneObject("SceneCamera");
        auto&camera = m_sceneCamera.AddComponent<Lutra::Camera>();
        camera.AspectRadio = (float)m_width / m_height;
        
        m_colorTexture = std::shared_ptr<Lutra::Texture>(new Lutra::Texture2D(m_width, m_height, Lutra::TextureFormat::RGBA8));
        auto depthTexute  = std::shared_ptr<Lutra::Texture>(new Lutra::Texture2D(m_width, m_height, Lutra::TextureFormat::D24S8));
        auto renderTexture = new Lutra::RenderTexture(m_width, m_height);
        renderTexture->ColorAttachments.push_back(m_colorTexture);
        renderTexture->DepthStencilAttachment = depthTexute;
        camera.RenderTarget_ = std::shared_ptr<Lutra::RenderTarget>(renderTexture);
        camera.RenderTarget_->SetClearColor(glm::vec4(0.1f, 0.1f, 0.1f, 1.0f));
        
        scene->GetEventDispatcher().Connect<Lutra::WindowResizeEvent>([this](Lutra::Event& event) -> bool{
            Lutra::WindowResizeEvent& winEvent = static_cast<Lutra::WindowResizeEvent&>(event);
            m_sceneCamera.GetComponent<Lutra::Camera>().AspectRadio = (float)winEvent.GetWidth() / winEvent.GetHeight();
            return true;
        });
    }

    SceneWindow::~SceneWindow()
    {
        
    }

    void SceneWindow::Open()
    {
        m_isOpen = true;
    }

    void SceneWindow::Close()
    {
        m_isOpen = false;
    }

    void SceneWindow::resize(uint32_t width, uint32_t height)
    {
        m_scene->GetEventDispatcher().Send(std::shared_ptr<Lutra::WindowResizeEvent>(new Lutra::WindowResizeEvent(width, height)));
        m_windowSize.x = width;
        m_windowSize.y = height;
    }

    void SceneWindow::OnGUI(uint32_t width, uint32_t height)
    {
        ImGui::SetNextWindowSize(ImVec2(800, 600), ImGuiCond_FirstUseEver);
        ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoCollapse;
        ImGui::Begin("Scene", &m_isOpen, window_flags);
        auto size = ImGui::GetContentRegionAvail();
        if (size.x != m_windowSize.x || size.y != m_windowSize.y) {
            resize(size.x, size.y);
        }

        ImGui::Image(reinterpret_cast<ImTextureID>(m_colorTexture->GetTextureID()), size);
        ImGui::End();
    }
}
