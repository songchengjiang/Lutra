//
//  Texture_GL.hpp
//  Lutra
//
//  Created by JasonCheng on 2021/1/7.
//

#ifndef DeviceTexture_GL_hpp
#define DeviceTexture_GL_hpp
#include "GLHeader.h"
#include "DeviceTexture.h"

namespace Lutra {

    class DeviceTextureGL : public DeviceTexture
    {
    public:
        
        virtual ~DeviceTextureGL() = default;

        virtual uint32_t GetID() const { return m_id; };
        
    protected:
        
        uint32_t m_id;
    };

    class DeviceTexture2DGL : public DeviceTextureGL
    {
    public:
        
        DeviceTexture2DGL(uint32_t width, uint32_t height, DeviceTextureForamt format);
        virtual ~DeviceTexture2DGL();

        virtual uint32_t GetWidth() const override { return m_width; }
        virtual uint32_t GetHeight() const override { return m_height; }

        virtual void SetData(const void* data, uint32_t size) override;

        virtual void Bind() const override;
        
    private:
        uint32_t m_width, m_height;
        GLenum m_internalFormat, m_dataFormat, m_dataType;
    };
}


#endif /* Texture_GL_hpp */
