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

    bool ResourceManager::DestroyResource(const sole::uuid& uuid)
    {
        ResourceManifest::Instance().SetUUIDAndPath(uuid, "");
        m_resources.erase(std::find_if(m_resources.begin(), m_resources.end(), [&](const std::shared_ptr<Resource>& resource) -> bool{
            return resource->GetUUID() == uuid;
        }));
        return true;
    }

    std::shared_ptr<Resource> ResourceManager::loadResource(const std::string& path)
    {
        auto uuid = ResourceManifest::Instance().FindUUID(path);
        if (uuid == sole::uuid{0, 0}) {
            return nullptr;
        }
        auto iter = std::find_if(m_resources.begin(), m_resources.end(), [&uuid](const std::shared_ptr<Resource>& resource) -> bool {
            return resource->GetUUID() == uuid;
        });
        if (iter != m_resources.end())
            return *iter;
        ReadStreamYaml stream(path);
        stream.Open();
        int Type = 0;
        stream.ReadValue("Type", Type);
        
        std::shared_ptr<Resource> resource;
        switch ((ResourceType)Type) {
            case ResourceType::Texture2D:
                resource = std::shared_ptr<Resource>(new Texture2D());
                break;
            case ResourceType::RenderTexture:
                resource = std::shared_ptr<Resource>(new RenderTexture());
                break;
            case ResourceType::Material:
                resource = std::shared_ptr<Resource>(new Material());
                break;
            case ResourceType::Mesh:
                resource = std::shared_ptr<Resource>(new Mesh());
                break;
                
            default:
                break;
        }
        resource->Deserialize(stream);
        stream.Close();
        m_resources.emplace_back(resource);
        return resource;
    }

    std::shared_ptr<Resource> ResourceManager::loadResource(const sole::uuid& uuid)
    {
        return loadResource(ResourceManifest::Instance().FindPath(uuid));
    }

    void ResourceManager::saveResource(const std::string& path, Resource* resource)
    {
        ResourceManifest::Instance().SetUUIDAndPath(resource->GetUUID(), path);
        
        WriteStreamYaml stream(path);
        stream.Open();
        stream.WriteValue("Type", (int)resource->GetResourceType());
        resource->Serialize(stream);
        stream.Close();
    }

}
