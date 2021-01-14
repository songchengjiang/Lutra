//
//  Texture.h
//  Lutra
//
//  Created by JasonCheng on 2021/1/7.
//

#ifndef DeviceTexture_h
#define DeviceTexture_h

namespace Lutra {

    enum DeviceTextureForamt
    {
        RGB8,
        RGBA8,
        
        D16,
        D24,
        D32F,
        D24S8,
        D32FS8,
    };

    enum class DeviceTextureFilter
    {
        Linear,
        Nearest,
    };

    enum class DeviceTextureWrap
    {
        Clamp,
        Repeat,
    };

    class DeviceTexture
    {
    public:
        
        virtual ~DeviceTexture() = default;

        virtual uint32_t GetWidth() const = 0;
        virtual uint32_t GetHeight() const = 0;

        virtual void SetData(const void* data, uint32_t size) = 0;
        
        virtual void SetFilter(DeviceTextureFilter filter) = 0;
        virtual DeviceTextureFilter GetFilter() const = 0;
        
        virtual void SetWrap(DeviceTextureWrap wrap) = 0;
        virtual DeviceTextureWrap GettWrap() const = 0;

        virtual void Bind() const = 0;
        
        virtual uint32_t GetID() const = 0;
        
    };
}



#endif /* Texture_h */
