//
//  ResourceManager.cpp
//  Lutra
//
//  Created by JasonCheng on 2021/1/10.
//

#include "ResourceManager.h"
#include "Texture.h"
#include "Material.h"
#include "Pass.h"
#include "Shader.h"
#include "Mesh.h"
#include "StreamYaml.h"
#include "ResourceManifest.h"

namespace Lutra {

    ResourceManager& ResourceManager::Instance()
    {
        static ResourceManager mgr;
        return mgr;
    }

    Resource* ResourceManager::loadResource(const std::string& path)
    {
        ReadStreamYaml stream(ResourceManifest::Instance().GetAbsolutePath(path));
        stream.Open();
        int Type = 0;
        stream.ReadValue("Type", Type);
        
        Resource *resource = nullptr;
        switch ((ResourceType)Type) {
            case ResourceType::Texture:
                resource = new Texture2D();
                break;
            case ResourceType::Material:
                resource = new Material();
                break;
            case ResourceType::Mesh:
                resource = new Mesh();
                break;
                
            default:
                break;
        }
        resource->Deserialize(stream);
        stream.Close();
        m_resources.emplace_back(resource);
        return resource;
    }

    Resource* ResourceManager::loadResource(const sole::uuid& uuid)
    {
        return loadResource(ResourceManifest::Instance().GetRelativePath(uuid));
    }

    void ResourceManager::saveResource(const std::string& path, Resource* resource)
    {
        ResourceManifest::Instance().SetUUIDAndPath(resource->GetUUID(), path);
        
        WriteStreamYaml stream(ResourceManifest::Instance().GetAbsolutePath(path));
        stream.Open();
        stream.BeginMap("");
        stream.WriteValue("Type", (int)resource->GetResourceType());
        resource->Serialize(stream);
        stream.EndMap();
        stream.Close();
    }

}
