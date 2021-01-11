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
#include "RenderTarget.h"
#include <set>

namespace Lutra {

    Scene::Scene()
    {
    }

    SceneObject Scene::CreateSceneObject(const std::string& name)
    {
        SceneObject so{m_registry.create(), this};
        so.AddComponent<Transform>();
        so.AddComponent<Tag>().Name = name;
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
        std::set<std::shared_ptr<RenderTarget>> renderTargets;
        
        auto view = m_registry.view<Transform, Camera>();
        view.each([&, this](Transform& trans, Camera& cam){
            if (cam.RenderTarget_ != nullptr) {
                if (cam.RenderTarget_->GetType() != RenderTargetType::Window) {
                    rttCameras.push_back(&cam);
                }else {
                    mainCameras.push_back(&cam);
                }
                renderTargets.insert(cam.RenderTarget_);
            }
        });
        
        auto renderFunc = [&rContext, &graphic, this](Camera &cam){
            if (cam.RenderTarget_ != nullptr) {
                uint32_t width = cam.RenderTarget_->GetWidth();
                uint32_t height = cam.RenderTarget_->GetHeight();
                rContext.Camera_ = &cam;
                graphic.SetRenderTarget(cam.RenderTarget_);
                graphic.SetViewport(cam.Viewport_.X * width,
                                    cam.Viewport_.Y * height,
                                    cam.Viewport_.Width * width - cam.Viewport_.X * width,
                                    cam.Viewport_.Height * height - cam.Viewport_.Y * height);
                
                for (auto& system : m_systemList) {
                    system->OnRender(rContext);
                }
                graphic.SetRenderTarget(cam.RenderTarget_, false);
            }
        };
        
        for (auto& rt : renderTargets) {
            graphic.SetRenderTarget(rt);
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
