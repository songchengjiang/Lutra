//
//  Scene.cpp
//  Lutra
//
//  Created by JasonCheng on 2021/1/4.
//

#include "Scene.h"
#include "SceneObject.h"
#include "Transform.h"
#include "Camera.h"
#include "Context.h"
#include "Graphic.h"
#include "RenderTarget.h"

namespace Lutra {

    Scene::Scene()
    {
    }

    SceneObject Scene::CreateSceneObject(const std::string& name)
    {
        SceneObject so{m_registry.create(), name, this};
        so.AddComponent<Transform>();
        return so;
    }

    void Scene::DestroySceneObject(SceneObject so)
    {
        m_registry.destroy(so);
    }

    void Scene::OnUpdate(const FrameContext& context, Graphic& graphic)
    {
        for (auto& system : m_systemList) {
            system->OnUpdate(context);
        }
        
        auto view = m_registry.view<Transform, Camera>();
        RenderContext rContext;
        rContext.Graphic_ = &graphic;
        std::vector<Camera*> rttCameras;
        std::vector<Camera*> mainCameras;
        view.each([&, this](Transform& trans, Camera& cam){
            if (cam.RenderTarget_->GetType() != RenderTargetType::Window) {
                rttCameras.push_back(&cam);
            }else {
                mainCameras.push_back(&cam);
            }
        });
        
        auto renderFunc = [&rContext, &graphic, this](Camera &cam){
            if (cam.RenderTarget_ != nullptr) {
                rContext.Camera_ = &cam;
                graphic.SetRenderTarget(cam.RenderTarget_);
                graphic.SetClearColor(cam.RenderTarget_->GetClearColor());
                graphic.Clear(ClearFlag::COLOR | ClearFlag::DEPTH | ClearFlag::STENCIL);
                
                for (auto& system : m_systemList) {
                    system->OnRender(rContext);
                }
            }
        };
        
        for (auto& camera : rttCameras) {
            renderFunc(*camera);
        }
        
        for (auto& camera : mainCameras) {
            renderFunc(*camera);
        }
    }
}
