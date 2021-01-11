//
//  TransformSystem.cpp
//  Lutra
//
//  Created by JasonCheng on 2021/1/5.
//

#include "TransformSystem.h"
#include "Components/Transform.h"
#include "Components/Camera.h"
#include "Components/SceneObjectDelegate.h"

namespace Lutra {
        
    TransformSystem::TransformSystem(Scene* scene)
    : System(scene)
    {
        
    }

    TransformSystem::~TransformSystem()
    {
        
    }

    static glm::mat4 GetLocalTransform(Transform& transform)
    {
        glm::mat4 rotation = glm::toMat4(glm::quat(glm::radians(transform.Rotation)));
        return glm::translate(glm::mat4(1.0f), transform.Position)
            * rotation
            * glm::scale(glm::mat4(1.0f), transform.Scale);
    }

    static glm::mat4 GetWorldTransform(const SceneObjectDelegate& sod)
    {
        if (!sod.GetParent())
            return sod.This.GetComponent<Transform>().LocalMat;
        return GetWorldTransform(sod.GetParent().GetComponent<SceneObjectDelegate>()) * sod.This.GetComponent<Transform>().LocalMat;
    }

    void TransformSystem::OnUpdate(const FrameContext& context)
    {
        auto view = registry().view<Transform, SceneObjectDelegate>();
        
        view.each([](Transform& trans, SceneObjectDelegate& sod){
            trans.LocalMat = GetLocalTransform(trans);
        });
        
        view.each([](Transform& trans, SceneObjectDelegate& sod){
            trans.WorldMat = GetWorldTransform(sod);
        });
        
        auto cView = registry().view<Transform, Camera>();
        
        cView.each([](Transform& trans, Camera& cam){
            
            cam.ViewMat = glm::inverse(trans.WorldMat);
            
            if (cam.ProjType == Camera::ProjectionType::Perspective)
            {
                cam.ProjectMat = glm::perspective(glm::radians(cam.Fov), cam.AspectRadio, cam.ZNear, cam.ZFar);
            }
            else
            {
                float orthoLeft   = -cam.OrthographicSize * cam.AspectRadio * 0.5f;
                float orthoRight  = cam.OrthographicSize * cam.AspectRadio * 0.5f;
                float orthoBottom = -cam.OrthographicSize * 0.5f;
                float orthoTop    = cam.OrthographicSize * 0.5f;

                cam.ProjectMat = glm::ortho(orthoLeft, orthoRight,
                    orthoBottom, orthoTop, cam.ZNear, cam.ZFar);
            }
        });
    }

}
