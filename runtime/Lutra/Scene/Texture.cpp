//
//  Texture.cpp
//  Lutra
//
//  Created by JasonCheng on 2021/1/7.
//

#include "Texture.h"

namespace Lutra {

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

    void Texture2D::SetData(uint8_t* data, uint32_t size)
    {
        m_data.insert(m_data.begin(), data, data + size);
    }

}
