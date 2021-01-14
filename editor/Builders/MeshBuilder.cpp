//
//  MeshBuilder.cpp
//  Editor
//
//  Created by JasonCheng on 2021/1/13.
//

#include "MeshBuilder.h"

namespace LutraEditor {

    std::shared_ptr<Lutra::Mesh> MeshBuilder::BuildPlane()
    {
        auto mesh = Lutra::ResourceManager::Instance().CreateResource<Lutra::Mesh>();
        mesh->Vertices.push_back(glm::vec3(0.0f, 0.0f, 0.0f));
        mesh->Vertices.push_back(glm::vec3(1.0f, 0.0f, 0.0f));
        mesh->Vertices.push_back(glm::vec3(0.0f, 1.0f, 0.0f));
        mesh->Vertices.push_back(glm::vec3(1.0f, 1.0f, 0.0f));
        
        mesh->Normals.push_back(glm::vec3(0.0f, 0.0f, 1.0f));
        mesh->Normals.push_back(glm::vec3(0.0f, 0.0f, 1.0f));
        mesh->Normals.push_back(glm::vec3(0.0f, 0.0f, 1.0f));
        mesh->Normals.push_back(glm::vec3(0.0f, 0.0f, 1.0f));
        
        mesh->Texcoord0.push_back(glm::vec2(0.0f, 0.0f));
        mesh->Texcoord0.push_back(glm::vec2(1.0f, 0.0f));
        mesh->Texcoord0.push_back(glm::vec2(0.0f, 1.0f));
        mesh->Texcoord0.push_back(glm::vec2(1.0f, 1.0f));
        
        mesh->SubMeshList.emplace_back(Lutra::SubMesh{0, {0, 1, 2, 1, 3, 2}});
        return mesh;
    }

    std::shared_ptr<Lutra::Mesh> MeshBuilder::BuildBox()
    {
        auto mesh = Lutra::ResourceManager::Instance().CreateResource<Lutra::Mesh>();
        mesh->Vertices.push_back(glm::vec3(0.0f, 0.0f, 0.0f));
        mesh->Vertices.push_back(glm::vec3(1.0f, 0.0f, 0.0f));
        mesh->Vertices.push_back(glm::vec3(0.0f, 1.0f, 0.0f));
        mesh->Vertices.push_back(glm::vec3(1.0f, 1.0f, 0.0f));
        
        mesh->Normals.push_back(glm::vec3(0.0f, 0.0f, 1.0f));
        mesh->Normals.push_back(glm::vec3(0.0f, 0.0f, 1.0f));
        mesh->Normals.push_back(glm::vec3(0.0f, 0.0f, 1.0f));
        mesh->Normals.push_back(glm::vec3(0.0f, 0.0f, 1.0f));
        
        mesh->Texcoord0.push_back(glm::vec2(0.0f, 0.0f));
        mesh->Texcoord0.push_back(glm::vec2(1.0f, 0.0f));
        mesh->Texcoord0.push_back(glm::vec2(0.0f, 1.0f));
        mesh->Texcoord0.push_back(glm::vec2(1.0f, 1.0f));
        
        mesh->SubMeshList.emplace_back(Lutra::SubMesh{0, {0, 1, 2, 1, 3, 2}});
        return mesh;
    }

    std::shared_ptr<Lutra::Mesh> MeshBuilder::BuildSphere()
    {
        auto mesh = Lutra::ResourceManager::Instance().CreateResource<Lutra::Mesh>();
        mesh->Vertices.push_back(glm::vec3(0.0f, 0.0f, 0.0f));
        mesh->Vertices.push_back(glm::vec3(1.0f, 0.0f, 0.0f));
        mesh->Vertices.push_back(glm::vec3(0.0f, 1.0f, 0.0f));
        mesh->Vertices.push_back(glm::vec3(1.0f, 1.0f, 0.0f));
        
        mesh->Normals.push_back(glm::vec3(0.0f, 0.0f, 1.0f));
        mesh->Normals.push_back(glm::vec3(0.0f, 0.0f, 1.0f));
        mesh->Normals.push_back(glm::vec3(0.0f, 0.0f, 1.0f));
        mesh->Normals.push_back(glm::vec3(0.0f, 0.0f, 1.0f));
        
        mesh->Texcoord0.push_back(glm::vec2(0.0f, 0.0f));
        mesh->Texcoord0.push_back(glm::vec2(1.0f, 0.0f));
        mesh->Texcoord0.push_back(glm::vec2(0.0f, 1.0f));
        mesh->Texcoord0.push_back(glm::vec2(1.0f, 1.0f));
        
        mesh->SubMeshList.emplace_back(Lutra::SubMesh{0, {0, 1, 2, 1, 3, 2}});
        return mesh;
    }

}
