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

    void ResourceManifest::SetResourcesRootPath(const std::string& path)
    {
        m_rootPath = path;
    }

    void ResourceManifest::Load(const std::string& path)
    {
        std::ifstream fs;
        fs.open(m_rootPath + path);
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
        fs.open(m_rootPath + path);
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
        m_uuidMapPath[uuid] = path;
        m_pathMapuuid[path] = uuid;
    }

    std::string ResourceManifest::GetAbsolutePath(const std::string& relPath)
    {
        return m_rootPath + relPath;
    }

    std::string ResourceManifest::GetRelativePath(const sole::uuid& uuid)
    {
        std::string path;
        auto iter = m_uuidMapPath.find(uuid);
        if (iter != m_uuidMapPath.end()) {
            path = iter->second;
        }
        return path;
    }

}
