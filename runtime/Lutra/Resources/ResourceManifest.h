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
        
        void SetRootDirectoty(const std::string& dir) { m_root = dir; }
        const std::string& GetRootDirectoty() const { return m_root; }
        
        void Load(const std::string& absPath);
        void Save(const std::string& absPath);
        
        void SetUUIDAndPath(const sole::uuid& uuid, const std::string& path);
        
        std::string FindPath(const sole::uuid& uuid);
        sole::uuid FindUUID(const std::string& path);
        
    private:
        
        std::unordered_map<sole::uuid, std::string> m_uuidMapPath;
        std::unordered_map<std::string, sole::uuid> m_pathMapuuid;
        std::string m_root;
    };

}
#endif /* ResourceManifest_hpp */
