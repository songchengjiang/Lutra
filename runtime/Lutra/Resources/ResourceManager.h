//
//  ResourceManager.hpp
//  Lutra
//
//  Created by JasonCheng on 2021/1/10.
//

#ifndef ResourceManager_hpp
#define ResourceManager_hpp
#include "Resource.h"

namespace Lutra {

    class ResourceManager
    {
    public:
        ResourceManager() = default;
        ~ResourceManager() = default;
        
        static ResourceManager& Instance();
        
        template<typename T>
        std::shared_ptr<T> LoadResource(const std::string& path);
        
        template<typename T>
        std::shared_ptr<T> LoadResource(const sole::uuid& uuid);
    
        template<typename T>
        void SaveResource(const std::string& path, const std::shared_ptr<T>& resource);
        
    private:
        
        Resource* loadResource(const std::string& path);
        Resource* loadResource(const sole::uuid& uuid);
        void saveResource(const std::string& path, Resource* resource);
        
    private:
        
        std::vector<std::shared_ptr<Resource>> m_resources;
    };

    template<typename T>
    std::shared_ptr<T> ResourceManager::LoadResource(const std::string& path)
    {
        return std::shared_ptr<T>(static_cast<T*>(loadResource(path)));
    }

    template<typename T>
    std::shared_ptr<T> ResourceManager::LoadResource(const sole::uuid& uuid)
    {
        return std::shared_ptr<T>(static_cast<T*>(loadResource(uuid)));
    }

    template<typename T>
    void ResourceManager::SaveResource(const std::string& path, const std::shared_ptr<T>& resource)
    {
        saveResource(path, resource.get());
    }

}

#endif /* ResourceManager_hpp */
