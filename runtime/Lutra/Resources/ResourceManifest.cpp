//
//  ResourceManifest.cpp
//  Lutra
//
//  Created by JasonCheng on 2021/1/10.
//

#include "ResourceManifest.h"
#include <fstream>

namespace Lutra {

    ResourceManifest& ResourceManifest::Instance()
    {
        static ResourceManifest manifest;
        return manifest;
    }

    void ResourceManifest::Load(const std::string& path)
    {
        m_uuidMapPath.clear();
        m_pathMapuuid.clear();
        std::ifstream fs;
        fs.open(path);
        if (fs.is_open()) {
            std::string line;
            while (std::getline(fs, line)) {
                auto pos = line.find_first_of(' ');
                auto uuid = sole::rebuild(line.substr(0, pos));
                auto path = line.substr(pos + 1);
                SetUUIDAndPath(uuid, path);
            }
        }
        fs.close();
    }

    void ResourceManifest::Save(const std::string& path)
    {
        std::ofstream fs;
        fs.open(path);
        if (fs.is_open()) {
            std::stringstream ss;
            for (auto& iter : m_uuidMapPath) {
                ss << iter.first << " " << iter.second << std::endl;
            }
            fs << ss.rdbuf();
        }
        fs.close();
    }

    void ResourceManifest::SetUUIDAndPath(const sole::uuid& uuid, const std::string& path)
    {
        if (path.empty()) {
            m_pathMapuuid.erase(m_uuidMapPath[uuid]);
            m_uuidMapPath.erase(uuid);
        }else {
            m_uuidMapPath[uuid] = path;
            m_pathMapuuid[path] = uuid;
        }
    }

    std::string ResourceManifest::FindPath(const sole::uuid& uuid)
    {
        std::string path;
        auto iter = m_uuidMapPath.find(uuid);
        if (iter != m_uuidMapPath.end()) {
            path = iter->second;
        }
        return path;
    }

    sole::uuid ResourceManifest::FindUUID(const std::string& path)
    {
        sole::uuid uuid{0, 0};
        auto iter = m_pathMapuuid.find(path);
        if (iter != m_pathMapuuid.end()) {
            uuid = iter->second;
        }
        return uuid;
        
    }

}
