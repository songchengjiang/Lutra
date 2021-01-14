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
        
        m_colorTexture = Lutra::ResourceManager::Instance().CreateResource<Lutra::RenderTexture>(m_width, m_height, Lutra::TextureFormat::RGBA8, Lutra::TextureFormat::D24S8);
        camera.RenderTexture_ = m_colorTexture;
        camera.RenderTexture_->SetClearColor(glm::vec4(0.1f, 0.1f, 0.1f, 1.0f));
        
        m_sceneCamera.GetComponent<Lutra::Transform>().Position.z = 2.0f;
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
        //m_scene->GetEventDispatcher().Send(std::shared_ptr<Lutra::WindowResizeEvent>(new Lutra::WindowResizeEvent(width, height)));
        m_sceneCamera.GetComponent<Lutra::Camera>().AspectRadio = (float)width / height;
        m_width = width;
        m_height = height;
    }

    void SceneWindow::OnGUI(uint32_t width, uint32_t height)
    {
        ImGui::SetNextWindowSize(ImVec2(800, 600), ImGuiCond_FirstUseEver);
        ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoCollapse;
        ImGui::Begin("Scene", &m_isOpen, window_flags);
        auto size = ImGui::GetContentRegionAvail();
        if (size.x != m_width || size.y != m_height) {
            resize(size.x, size.y);
        }

        ImGui::Image(reinterpret_cast<ImTextureID>(m_colorTexture->GetTextureID()), size);
        ImGui::End();
    }
}
