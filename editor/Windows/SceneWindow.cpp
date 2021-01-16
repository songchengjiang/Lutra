//
//  SceneWindow.cpp
//  Editor
//
//  Created by JasonCheng on 2021/1/8.
//

#include "SceneWindow.h"
#include "MeshBuilder.h"
#include "MaterialBuilder.h"
#include "Utils/MathHelper.h"

static const uint32_t REF_GRID_SO   = 0x10000000;
static const uint32_t REF_CAMERA_SO = 0x20000000;

namespace LutraEditor {

    SceneWindow::SceneWindow(const std::shared_ptr<Lutra::Scene>& scene)
    : m_scene(scene)
    , m_width(1280)
    , m_height(720)
    {
        m_sceneCamera = m_scene->CreateSceneObject("SceneCamera");
        auto&camera = m_sceneCamera.AddComponent<Lutra::Camera>();
        camera.VisibleMask |= REF_GRID_SO | REF_CAMERA_SO;
        camera.AspectRadio = (float)m_width / m_height;
        camera.ZNear = 0.1f;
        camera.ZFar  = 1000.0f;
        
        m_colorTexture = Lutra::ResourceManager::Instance().CreateResource<Lutra::RenderTexture>(m_width, m_height, Lutra::TextureFormat::RGBA8, Lutra::TextureFormat::D24S8);
        camera.RenderTexture_ = m_colorTexture;
        camera.RenderTexture_->SetClearColor(glm::vec4(0.1f, 0.1f, 0.1f, 1.0f));
        
        m_cameraManipulator.reset(new CameraManipulator(m_sceneCamera, glm::vec3(0.0f, 0.0f, 50.0f), glm::vec3(-30.0f, 0.0f, 0.0f)));
        
        createReferenceGrid();
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
        
        m_cameraManipulator->Update(width, height);
        ImGui::Image(reinterpret_cast<ImTextureID>(m_colorTexture->GetTextureID()), size, {0, 1}, {1, 0});
        ImGui::End();
    }

    void SceneWindow::createReferenceGrid()
    {
        auto gridSO  = m_scene->CreateSceneObject("ReferenceGrid");
        gridSO.GetComponent<Lutra::Tag>().VisibleTag = REF_GRID_SO;
        auto& mf = gridSO.AddComponent<Lutra::MeshFilter>();
        mf.MeshPtr = GridMeshBuilder::Build(50.0f, 50.0f);
        gridSO.AddComponent<Lutra::MeshRenderer>().Materials.push_back(UnlintColorMaterialBuilder::Build());
    }

    void SceneWindow::OnNotifySceneObjectAdded(Lutra::SceneObject so)
    {
        if (so.HasComponent<Lutra::Serializable>() && so.HasComponent<Lutra::Camera>()) {
            auto& camera = so.GetComponent<Lutra::Camera>();
            auto cameraVisual  = m_scene->CreateSceneObject("CameraVisual");
            cameraVisual.GetComponent<Lutra::Tag>().VisibleTag = REF_CAMERA_SO;
            auto& mf = cameraVisual.AddComponent<Lutra::MeshFilter>();
            mf.MeshPtr = ViewportMeshBuilder::Build(camera.Fov, camera.AspectRadio, camera.OrthographicSize, camera.ZNear, camera.ZFar, camera.ProjType == Lutra::Camera::ProjectionType::Perspective, glm::vec4(114.0f / 255.0f, 147.0f / 255.0f, 187.0f / 255.0f, 1.0f));
            auto material = UnlintColorMaterialBuilder::Build();
            material->GetPass(0)->GetBlendMode() = Lutra::BlendMode::Normal;
            cameraVisual.AddComponent<Lutra::MeshRenderer>().Materials.push_back(material);
            
            so.GetComponent<Lutra::SceneObjectDelegate>().AddChild(cameraVisual);
            
            m_cameraMap[so] = cameraVisual;
        }
    }

    void SceneWindow::OnNotifySceneObjectPropoertyChanged(Lutra::SceneObject so)
    {
        if (so.HasComponent<Lutra::Serializable>() && so.HasComponent<Lutra::Camera>()) {
            auto& camera = so.GetComponent<Lutra::Camera>();
            auto& cameraVisual = m_cameraMap[so];
            auto& mf = cameraVisual.GetComponent<Lutra::MeshFilter>();
            mf.MeshPtr = ViewportMeshBuilder::Build(camera.Fov, camera.AspectRadio, camera.OrthographicSize, camera.ZNear, camera.ZFar, camera.ProjType == Lutra::Camera::ProjectionType::Perspective, glm::vec4(114.0f / 255.0f, 147.0f / 255.0f, 187.0f / 255.0f, 1.0f));
            
//            glm::vec3 scale;
//            glm::quat rotation;
//            glm::vec3 translation;
//            MathHelper::decompose(so.GetComponent<Lutra::Transform>().WorldMat, &translation, &rotation, &scale);
//
//            cameraVisual.GetComponent<Lutra::Transform>().Position = translation;
//            cameraVisual.GetComponent<Lutra::Transform>().Rotation = glm::degrees(glm::eulerAngles(rotation));
//            cameraVisual.GetComponent<Lutra::Transform>().Scale    = scale;
        }
    }

    void SceneWindow::OnNotifySceneObjectRemoved(Lutra::SceneObject so)
    {
//        if (so.HasComponent<Lutra::Serializable>() && so.HasComponent<Lutra::Camera>()) {
//            so.GetComponent<Lutra::SceneObjectDelegate>().RemoveChild(m_cameraMap[so]);
//            m_cameraMap.erase(so);
//            m_scene->DestroySceneObject(m_cameraMap[so]);
//        }
    }
}
