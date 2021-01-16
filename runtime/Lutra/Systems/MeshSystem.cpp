//
//  MeshSystem.cpp
//  Lutra
//
//  Created by JasonCheng on 2021/1/14.
//

#include "MeshSystem.h"
#include "Tag.h"
#include "Transform.h"
#include "Camera.h"
#include "Context.h"
#include "Graphic.h"
#include "MeshFilter.h"
#include "Mesh.h"
#include "SceneObjectDelegate.h"
#include "MeshRenderer.h"

namespace Lutra {
        
    MeshSystem::MeshSystem(Scene* scene)
    : System(scene)
    {
    }

    MeshSystem::~MeshSystem()
    {
        
    }

    void MeshSystem::OnUpdate(const FrameContext& context)
    {
    }
    
    void MeshSystem::OnRender(const RenderContext& context)
    {
        auto view = registry().view<Tag, Transform, SceneObjectDelegate, MeshRenderer, MeshFilter>();
        view.each([&](Tag& tag, Transform& trans, SceneObjectDelegate &sod, MeshRenderer& mr, MeshFilter& mf){
            if (tag.VisibleTag & context.Camera_->VisibleMask) {
                if (sod.IsVisibleInherit() && !mr.Materials.empty() && mf.MeshPtr != nullptr) {
                    for (size_t i = 0; i < mf.MeshPtr->SubMeshList.size(); ++i) {
                        auto& subMesh = mf.MeshPtr->SubMeshList[i];
                        if (subMesh.MaterialIndex < mr.Materials.size()) {
                            if (mr.Materials[subMesh.MaterialIndex] != nullptr)
                                context.Graphic_->DrawMesh(mf.MeshPtr.get(), mr.Materials[subMesh.MaterialIndex].get(), context.Camera_, i, 0, trans.WorldMat);
                        }
                    }
                }
            }
        });
    }

}
