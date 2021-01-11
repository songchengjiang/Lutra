//
//  SpriteSystem.cpp
//  Lutra
//
//  Created by JasonCheng on 2021/1/5.
//

#include "SpriteSystem.h"
#include "SpriteRenderer.h"
#include "SceneObjectDelegate.h"
#include "Transform.h"
#include "Context.h"
#include "Graphic.h"
#include "MeshFilter.h"
#include "Mesh.h"
#include "Camera.h"

namespace Lutra {
        
    SpriteSystem::SpriteSystem(Scene* scene)
    : System(scene)
    {
        
    }

    SpriteSystem::~SpriteSystem()
    {
        
    }

    void SpriteSystem::OnUpdate(const FrameContext& context)
    {
    }
    
    void SpriteSystem::OnRender(const RenderContext& context)
    {
        auto view = registry().view<Transform, SceneObjectDelegate, SpriteRenderer, MeshFilter>();
        view.each([&](Transform& trans, SceneObjectDelegate &sod, SpriteRenderer& sr, MeshFilter& mf){
            if (sod.IsVisibleInherit()) {
                glm::vec2 pos = (glm::vec2(1.0f) - sr.Anchor) * glm::vec2(0.0f) + sr.Anchor * sr.Size;
                glm::mat4 modelMat = trans.WorldMat * glm::translate(glm::mat4(1.0f), glm::vec3(-pos, 0.0f)) * glm::scale(glm::mat4(1.0f), glm::vec3(sr.Size.x, sr.Size.y, 1.0f));
                for (size_t i = 0; i < mf.MeshPtr->SubMeshList.size(); ++i) {
                    auto& subMesh = mf.MeshPtr->SubMeshList[i];
                    if (subMesh.MaterialIndex < sr.Materials.size()) {
                        context.Graphic_->DrawMesh(mf.MeshPtr.get(), sr.Materials[subMesh.MaterialIndex].get(), context.Camera_, i, 0, modelMat);
                    }
                }
            }
        });
    }

}
