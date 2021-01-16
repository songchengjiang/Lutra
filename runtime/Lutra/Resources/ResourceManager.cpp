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

    ResourceManager::ResourceManager()
    {
        m_resourceFactorys[".tex"].reset(new ResourceFactoryImp<Texture2D>);
        m_resourceFactorys[".rt"].reset(new ResourceFactoryImp<RenderTexture>);
        m_resourceFactorys[".mesh"].reset(new ResourceFactoryImp<Mesh>);
        m_resourceFactorys[".mat"].reset(new ResourceFactoryImp<Material>);
    }

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
        
        std::string ext = path.substr(path.find_last_of("."));
        std::shared_ptr<Resource> resource = m_resourceFactorys[ext]->Create();
        ReadStreamYaml stream(path);
        stream.Open();
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
        resource->Serialize(stream);
        stream.Close();
    }

}
