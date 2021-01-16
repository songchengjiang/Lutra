//
//  ResourceManager.hpp
//  Lutra
//
//  Created by JasonCheng on 2021/1/10.
//

#ifndef ResourceManager_hpp
#define ResourceManager_hpp
#include "Resource.h"
#include "ResourceFactory.h"
#include <unordered_map>

namespace Lutra {

    class Scene;
    class ResourceManager
    {
    public:
        ResourceManager();
        ~ResourceManager() = default;
        
        static ResourceManager& Instance();
        
        template<typename T, typename... Args>
        std::shared_ptr<T> CreateResource(Args&&... args);
        
        bool DestroyResource(const sole::uuid& uuid);
        
        template<typename T>
        std::shared_ptr<T> LoadResource(const std::string& path);
        
        template<typename T>
        std::shared_ptr<T> LoadResource(const sole::uuid& uuid);
        
    
        template<typename T>
        void SaveResource(const std::string& path, const std::shared_ptr<T>& resource);
        
    private:
        
        std::shared_ptr<Resource> loadResource(const std::string& path);
        std::shared_ptr<Resource> loadResource(const sole::uuid& uuid);
        void saveResource(const std::string& path, Resource* resource);
        
    private:
        
        std::vector<std::shared_ptr<Resource>> m_resources;
        std::unordered_map<std::string, std::unique_ptr<ResourceFactory>> m_resourceFactorys;
    };

    template<typename T, typename... Args>
    std::shared_ptr<T> ResourceManager::CreateResource(Args&&... args)
    {
        auto res = std::shared_ptr<T>(new T(std::forward<Args>(args)...));
        m_resources.emplace_back(res);
        return res;
    }

    template<typename T>
    std::shared_ptr<T> ResourceManager::LoadResource(const std::string& path)
    {
        return std::dynamic_pointer_cast<T>(loadResource(path));
    }

    template<typename T>
    std::shared_ptr<T> ResourceManager::LoadResource(const sole::uuid& uuid)
    {
        return std::dynamic_pointer_cast<T>(loadResource(uuid));
    }

    template<typename T>
    void ResourceManager::SaveResource(const std::string& path, const std::shared_ptr<T>& resource)
    {
        saveResource(path, resource.get());
    }

}

#endif /* ResourceManager_hpp */
