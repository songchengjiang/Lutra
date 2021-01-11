//
//  TextureLoader.cpp
//  Editor
//
//  Created by JasonCheng on 2021/1/9.
//

#include "IconManager.h"

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
        
        GLenum internalFormat = channels == 3? GL_RGB8: GL_RGBA8;
        GLenum dataFormat = channels == 3? GL_RGB: GL_RGBA;
        GLuint texID;
        glGenTextures(1, &texID);
        glBindTexture(GL_TEXTURE_2D, texID);
        glTexImage2D(GL_TEXTURE_2D, 0,  internalFormat, width, height, 0, dataFormat, GL_UNSIGNED_BYTE, data);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        m_textures[type] = texID;
    }

}
