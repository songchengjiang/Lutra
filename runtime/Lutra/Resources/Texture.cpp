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
        m_resourceType = ResourceType::Texture;
    }

    Texture2D::Texture2D()
    : m_width(0)
    , m_height(0)
    , m_format(TextureFormat::RGBA8)
    {
        
    }

    Texture2D::Texture2D(int width, int height, TextureFormat format)
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
        stream.WriteValue("TextureType", (int)m_type);
        stream.WriteValue("TextureFormat", (int)m_format);
        stream.WriteValue("Width", m_width);
        stream.WriteValue("Height", m_height);
        stream.WriteValue("Data", m_data);
    }

    void Texture2D::Deserialize(ReadStream& stream)
    {
        Texture::Deserialize(stream);
        int type;
        stream.ReadValue("TextureType", type); m_type = (TextureType)type;
        stream.ReadValue("TextureFormat", type); m_format = (TextureFormat)type;
        stream.ReadValue("Width", m_width);
        stream.ReadValue("Height", m_height);
        stream.ReadValue("Data", m_data);
    }

    void Texture2D::SetData(uint8_t* data, uint32_t size)
    {
        m_data.insert(m_data.begin(), data, data + size);
    }

}
