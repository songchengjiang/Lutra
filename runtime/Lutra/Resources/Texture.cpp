//
//  Texture.cpp
//  Lutra
//
//  Created by JasonCheng on 2021/1/7.
//

#include "Texture.h"
#include "Stream.h"

namespace Lutra {

    Texture::Texture()
    {
    }

    void Texture::Serialize(WriteStream& stream)
    {
        Resource::Serialize(stream);
        stream.WriteValue("TextureType", (int)m_type);
        stream.WriteValue("TextureFilter", (int)m_filter);
        stream.WriteValue("TextureWrap", (int)m_wrap);
    }

    void Texture::Deserialize(ReadStream& stream)
    {
        Resource::Deserialize(stream);
        int type;
        stream.ReadValue("TextureType", type); m_type = (TextureType)type;
        stream.ReadValue("TextureFilter", type); m_filter = (TextureFilter)type;
        stream.ReadValue("TextureWrap", type); m_wrap = (TextureWrap)type;
    }

    Texture2D::Texture2D()
    : m_width(0)
    , m_height(0)
    , m_format(TextureFormat::RGBA8)
    {
        
    }

    Texture2D::Texture2D(uint32_t width, uint32_t height, TextureFormat format)
    : m_width(width)
    , m_height(height)
    , m_format(format)
    {
        m_type = TextureType::TEX2D;
    }

    Texture2D::~Texture2D()
    {
        
    }

    void Texture2D::Serialize(WriteStream& stream)
    {
        Texture::Serialize(stream);
        stream.WriteValue("Format", (int)m_format);
        stream.WriteValue("Width", m_width);
        stream.WriteValue("Height", m_height);
        stream.WriteValue("Data", m_data);
    }

    void Texture2D::Deserialize(ReadStream& stream)
    {
        Texture::Deserialize(stream);
        int format;
        stream.ReadValue("Format", format); m_format = (TextureFormat)format;
        stream.ReadValue("Width", m_width);
        stream.ReadValue("Height", m_height);
        stream.ReadValue("Data", m_data);
    }

    void Texture2D::SetData(uint8_t* data, uint32_t size)
    {
        m_data.insert(m_data.begin(), data, data + size);
    }

    RenderTexture::RenderTexture()
    : m_width(0)
    , m_height(0)
    , m_colorFormat(TextureFormat::RGBA8)
    , m_depthFormat(TextureFormat::D24S8)
    {
        
    }

    RenderTexture::RenderTexture(uint32_t width, uint32_t height, TextureFormat colorFormat, TextureFormat depthFormat)
    : m_width(width)
    , m_height(height)
    , m_colorFormat(colorFormat)
    , m_depthFormat(depthFormat)
    {
        m_type = TextureType::RENDER;
    }

    RenderTexture::~RenderTexture()
    {
        
    }

    void RenderTexture::Serialize(WriteStream& stream)
    {
        Texture::Serialize(stream);
        stream.WriteValue("Width", m_width);
        stream.WriteValue("Height", m_height);
        stream.WriteValue("ColorFormat", (int)m_colorFormat);
        stream.WriteValue("DepthFormat", (int)m_depthFormat);
        stream.WriteValue("ClearColor", m_clearColor);
    }

    void RenderTexture::Deserialize(ReadStream& stream)
    {
        Texture::Deserialize(stream);
        stream.ReadValue("Width", m_width);
        stream.ReadValue("Height", m_height);
        int format;
        stream.ReadValue("ColorFormat", format); m_colorFormat = (TextureFormat)format;
        stream.ReadValue("DepthFormat", format); m_depthFormat = (TextureFormat)format;
        stream.ReadValue("ClearColor", m_clearColor);
    }
}
