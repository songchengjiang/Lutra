//
//  MeshFilterSystem.cpp
//  Lutra
//
//  Created by JasonCheng on 2021/1/7.
//

#include "MeshFilterSystem.h"
#include "Transform.h"
#include "MeshFilter.h"
#include "Mesh.h"

namespace Lutra {
        
    MeshFilterSystem::MeshFilterSystem(Scene* scene)
    : System(scene)
    {
    }

    MeshFilterSystem::~MeshFilterSystem()
    {
        
    }

    void MeshFilterSystem::OnUpdate(const FrameContext& context)
    {
        auto view = registry().view<Transform, MeshFilter>();
        
        view.each([&](Transform& trans, MeshFilter& mf){
            auto& mesh = mf.MeshPtr;
            if (mf.MeshPtr != nullptr && mesh->MarkForUpdate) {
                mesh->SequentialBuffer.clear();
                for (size_t i = 0; i < mesh->Vertices.size(); ++i) {
                    mesh->SequentialBuffer.push_back(mesh->Vertices[i].x);
                    mesh->SequentialBuffer.push_back(mesh->Vertices[i].y);
                    mesh->SequentialBuffer.push_back(mesh->Vertices[i].z);
                    if (!mesh->Normals.empty()) {
                        mesh->SequentialBuffer.push_back(mesh->Normals[i].x);
                        mesh->SequentialBuffer.push_back(mesh->Normals[i].y);
                        mesh->SequentialBuffer.push_back(mesh->Normals[i].z);
                    }
                    if (!mesh->Tangents.empty()) {
                        mesh->SequentialBuffer.push_back(mesh->Tangents[i].x);
                        mesh->SequentialBuffer.push_back(mesh->Tangents[i].y);
                        mesh->SequentialBuffer.push_back(mesh->Tangents[i].z);
                    }
                    if (!mesh->Texcoord0.empty()) {
                        mesh->SequentialBuffer.push_back(mesh->Texcoord0[i].x);
                        mesh->SequentialBuffer.push_back(mesh->Texcoord0[i].y);
                    }
                    if (!mesh->Texcoord1.empty()) {
                        mesh->SequentialBuffer.push_back(mesh->Texcoord1[i].x);
                        mesh->SequentialBuffer.push_back(mesh->Texcoord1[i].y);
                    }
                }
            }
        });
    }

}
