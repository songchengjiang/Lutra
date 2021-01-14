//
//  TextureManager.cpp
//  Editor
//
//  Created by JasonCheng on 2021/1/12.
//

#include "TextureManager.h"

namespace LutraEditor {

    static GLenum ConvertTextureForamtToInternalFormat(Lutra::TextureFormat format)
    {
        switch (format) {
            case Lutra::TextureFormat::RGB8: return GL_RGB8;
            case Lutra::TextureFormat::RGBA8: return GL_RGBA8;
            case Lutra::TextureFormat::D16: return GL_DEPTH_COMPONENT16;
            case Lutra::TextureFormat::D24: return GL_DEPTH_COMPONENT24;
            case Lutra::TextureFormat::D32F: return GL_DEPTH_COMPONENT32F;
            case Lutra::TextureFormat::D24S8: return GL_DEPTH24_STENCIL8;
            case Lutra::TextureFormat::D32FS8: return GL_DEPTH32F_STENCIL8;
        }
        return 0;
    }

    static GLenum ConvertTextureForamtToFormat(Lutra::TextureFormat format)
    {
        switch (format) {
            case Lutra::TextureFormat::RGB8: return GL_RGB;
            case Lutra::TextureFormat::RGBA8: return GL_RGBA;
            case Lutra::TextureFormat::D16:
            case Lutra::TextureFormat::D24:
            case Lutra::TextureFormat::D32F: return GL_DEPTH_COMPONENT;
            case Lutra::TextureFormat::D24S8:
            case Lutra::TextureFormat::D32FS8: return GL_DEPTH_STENCIL;
        }
        return 0;
    }

    static GLenum ConvertTextureForamtToDataType(Lutra::TextureFormat format)
    {
        switch (format) {
            case Lutra::TextureFormat::RGB8:
            case Lutra::TextureFormat::RGBA8: return GL_UNSIGNED_BYTE;
            case Lutra::TextureFormat::D16:
            case Lutra::TextureFormat::D24: return GL_UNSIGNED_INT;
            case Lutra::TextureFormat::D32F: return GL_FLOAT;
            case Lutra::TextureFormat::D24S8: return GL_UNSIGNED_INT_24_8;
            case Lutra::TextureFormat::D32FS8: return GL_FLOAT;
        }
        return 0;
    }

    TextureManager& TextureManager::Instance()
    {
        static TextureManager texMgr;
        return texMgr;
    }

    uint32_t TextureManager::CreateTextue(uint32_t width, uint32_t height, Lutra::TextureFormat format, const uint8_t* data)
    {
        GLuint texID;
        glGenTextures(1, &texID);
        glBindTexture(GL_TEXTURE_2D, texID);
        glTexImage2D(GL_TEXTURE_2D, 0,  ConvertTextureForamtToInternalFormat(format), width, height, 0, ConvertTextureForamtToFormat(format), ConvertTextureForamtToDataType(format), data);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        m_textures.push_back(texID);
        return texID;
    }

}
