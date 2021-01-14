//
//  Scene.cpp
//  Lutra
//
//  Created by JasonCheng on 2021/1/4.
//

#include "Scene.h"
#include "SceneObject.h"
#include "Transform.h"
#include "Tag.h"
#include "SceneObjectDelegate.h"
#include "Camera.h"
#include "Context.h"
#include "Graphic.h"
#include "Texture.h"
#include <set>

namespace Lutra {

    Scene::Scene()
    {
    }

    Scene::~Scene()
    {
        
    }

    SceneObject Scene::CreateSceneObject(const std::string& name)
    {
        SceneObject so{m_registry.create(), this};
        so.AddComponent<Tag>().Name = name;
        so.AddComponent<Transform>();
        so.AddComponent<SceneObjectDelegate>().This = so;
        m_sceneObjectList.push_back(so);
        return so;
    }

    void Scene::DestroySceneObject(SceneObject so)
    {
        m_sceneObjectList.erase(std::find(m_sceneObjectList.begin(), m_sceneObjectList.end(), so));
        m_registry.destroy(so);
    }

    void Scene::OnUpdate(const FrameContext& context, Graphic& graphic)
    {
        m_eventDispatcher.Dispatch();
        
        for (auto& system : m_systemList) {
            system->OnUpdate(context);
        }
        
        RenderContext rContext;
        rContext.Graphic_ = &graphic;
        std::vector<Camera*> rttCameras;
        std::vector<Camera*> mainCameras;
        std::set<std::shared_ptr<RenderTexture>> renderTextures;
        
        auto view = m_registry.view<Transform, Camera>();
        view.each([&, this](Transform& trans, Camera& cam){
            if (cam.RenderTexture_ != nullptr) {
                rttCameras.push_back(&cam);
                renderTextures.insert(cam.RenderTexture_);
            }else {
                mainCameras.push_back(&cam);
            }
        });
        
        auto renderFunc = [&rContext, &graphic, this](Camera &cam){
            if (cam.RenderTexture_ != nullptr) {
                uint32_t width = cam.RenderTexture_->GetWidth();
                uint32_t height = cam.RenderTexture_->GetHeight();
                rContext.Camera_ = &cam;
                graphic.SetRenderTexture(cam.RenderTexture_);
                graphic.SetViewport(cam.Viewport_.X * width,
                                    cam.Viewport_.Y * height,
                                    cam.Viewport_.Width * width - cam.Viewport_.X * width,
                                    cam.Viewport_.Height * height - cam.Viewport_.Y * height);
                
                for (auto& system : m_systemList) {
                    system->OnRender(rContext);
                }
                graphic.SetRenderTexture(cam.RenderTexture_, false);
            }
        };
        
        for (auto& rt : renderTextures) {
            graphic.SetRenderTexture(rt);
            graphic.SetViewport(0, 0, rt->GetWidth(), rt->GetHeight());
            graphic.SetClearColor(rt->GetClearColor());
            graphic.Clear(ClearFlag::COLOR | ClearFlag::DEPTH | ClearFlag::STENCIL);
        }
        
        for (auto& camera : rttCameras) {
            renderFunc(*camera);
        }
        
        for (auto& camera : mainCameras) {
            renderFunc(*camera);
        }
    }
}
