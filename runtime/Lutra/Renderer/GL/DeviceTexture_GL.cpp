//
//  Texture_GL.cpp
//  Lutra
//
//  Created by JasonCheng on 2021/1/7.
//

#include "DeviceTexture_GL.h"
#include "LutraAssert.h"

namespace Lutra {

    static GLenum ConvertDeviceTextureForamtToInternalFormat(DeviceTextureForamt format)
    {
        switch (format) {
            case DeviceTextureForamt::RGB8: return GL_RGB8;
            case DeviceTextureForamt::RGBA8: return GL_RGBA8;
            case DeviceTextureForamt::D16: return GL_DEPTH_COMPONENT16;
            case DeviceTextureForamt::D24: return GL_DEPTH_COMPONENT24;
            case DeviceTextureForamt::D32F: return GL_DEPTH_COMPONENT32F;
            case DeviceTextureForamt::D24S8: return GL_DEPTH24_STENCIL8;
            case DeviceTextureForamt::D32FS8: return GL_DEPTH32F_STENCIL8;
        }
        return 0;
    }

    static GLenum ConvertDeviceTextureForamtToFormat(DeviceTextureForamt format)
    {
        switch (format) {
            case DeviceTextureForamt::RGB8: return GL_RGB;
            case DeviceTextureForamt::RGBA8: return GL_RGBA;
            case DeviceTextureForamt::D16:
            case DeviceTextureForamt::D24:
            case DeviceTextureForamt::D32F: return GL_DEPTH_COMPONENT;
            case DeviceTextureForamt::D24S8:
            case DeviceTextureForamt::D32FS8: return GL_DEPTH_STENCIL;
        }
        return 0;
    }

    static GLenum ConvertDeviceTextureForamtToDataType(DeviceTextureForamt format)
    {
        switch (format) {
            case DeviceTextureForamt::RGB8:
            case DeviceTextureForamt::RGBA8: return GL_UNSIGNED_BYTE;
            case DeviceTextureForamt::D16:
            case DeviceTextureForamt::D24: return GL_UNSIGNED_INT;
            case DeviceTextureForamt::D32F: return GL_FLOAT;
            case DeviceTextureForamt::D24S8: return GL_UNSIGNED_INT_24_8;
            case DeviceTextureForamt::D32FS8: return GL_FLOAT;
        }
        return 0;
    }
    
    static GLenum ConvertDeviceFilterToFilter(DeviceTextureFilter filter)
    {
        switch (filter) {
            case DeviceTextureFilter::Linear: return GL_LINEAR;
            case DeviceTextureFilter::Nearest: return GL_NEAREST;
        }
        return 0;
    }

    static GLenum ConvertDeviceWrapToWrap(DeviceTextureWrap wrap)
    {
        switch (wrap) {
            case DeviceTextureWrap::Clamp: return GL_CLAMP_TO_EDGE;
            case DeviceTextureWrap::Repeat: return GL_REPEAT;
        }
        return 0;
    }

    DeviceTexture2DGL::DeviceTexture2DGL(uint32_t width, uint32_t height, DeviceTextureForamt format)
        : m_width(width)
        , m_height(height)
    {
        m_internalFormat = ConvertDeviceTextureForamtToInternalFormat(format);
        m_dataFormat     = ConvertDeviceTextureForamtToFormat(format);
        m_dataType       = ConvertDeviceTextureForamtToDataType(format);
        glGenTextures(1, &m_id);
        glBindTexture(GL_TEXTURE_2D, m_id);
        glTexImage2D(GL_TEXTURE_2D, 0, m_internalFormat, m_width, m_height, 0, m_dataFormat, m_dataType, nullptr);
        //glTexStorage2D(GL_TEXTURE_2D, 1, m_internalFormat, m_width, m_height);
        SetFilter(m_filter);
        SetWrap(m_wrap);
    }

    DeviceTexture2DGL::~DeviceTexture2DGL()
    {
        glDeleteTextures(1, &m_id);
    }

    void DeviceTexture2DGL::SetData(const void* data, uint32_t size)
    {
        glBindTexture(GL_TEXTURE_2D, m_id);
        glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, m_width, m_height, m_dataFormat, m_dataType, data);
    }

    void DeviceTexture2DGL::SetFilter(DeviceTextureFilter filter)
    {
        glBindTexture(GL_TEXTURE_2D, m_id);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, ConvertDeviceFilterToFilter(filter));
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, ConvertDeviceFilterToFilter(filter));
        glBindTexture(GL_TEXTURE_2D, 0);
        m_filter = filter;
    }

    void DeviceTexture2DGL::SetWrap(DeviceTextureWrap wrap)
    {
        glBindTexture(GL_TEXTURE_2D, m_id);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, ConvertDeviceWrapToWrap(wrap));
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, ConvertDeviceWrapToWrap(wrap));
        glBindTexture(GL_TEXTURE_2D, 0);
        m_wrap = wrap;
    }

    void DeviceTexture2DGL::Bind()
    {
        glBindTexture(GL_TEXTURE_2D, m_id);
        UpdateUsage();
    }
}
