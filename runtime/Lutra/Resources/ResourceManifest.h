//
//  ResourceManifest.hpp
//  Lutra
//
//  Created by JasonCheng on 2021/1/10.
//

#ifndef ResourceManifest_hpp
#define ResourceManifest_hpp

#include <string>
#include <unordered_map>
#include "sole.hpp"

namespace Lutra {

    class ResourceManifest
    {
    public:
        ResourceManifest() = default;
        ~ResourceManifest() = default;
        
        static ResourceManifest& Instance();
        
        void SetResourcesRootPath(const std::string& path);
        const std::string& GetResourcesRootPath() const { return m_rootPath; }
        
        void Load(const std::string& relPath);
        void Save(const std::string& relPath);
        
        void SetUUIDAndPath(const sole::uuid& uuid, const std::string& path);
        
        std::string GetAbsolutePath(const std::string& relPath);
        std::string GetRelativePath(const sole::uuid& uuid);
        
    private:
        
        std::unordered_map<sole::uuid, std::string> m_uuidMapPath;
        std::unordered_map<std::string, sole::uuid> m_pathMapuuid;
        std::string m_rootPath;
    };

}
#endif /* ResourceManifest_hpp */
