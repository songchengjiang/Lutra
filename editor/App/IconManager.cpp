//
//  TextureLoader.cpp
//  Editor
//
//  Created by JasonCheng on 2021/1/9.
//

#include "IconManager.h"
#include "TextureManager.h"

namespace LutraEditor {

    IconManager& IconManager::Instance()
    {
        static IconManager iconMgr;
        return iconMgr;
    }

    void IconManager::LoadTextue(IconType type, const std::string& path)
    {
        int width, height, channels;
        stbi_uc* data = nullptr;
        {
            data = stbi_load(path.c_str(), &width, &height, &channels, 0);
        }
        
        m_textures[type] = TextureManager::Instance().CreateTextue(width, height, channels == 3? Lutra::TextureFormat::RGB8: Lutra::TextureFormat::RGBA8, data);
    }

}
