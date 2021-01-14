//
//  Texture.hpp
//  Lutra
//
//  Created by JasonCheng on 2021/1/7.
//

#ifndef Texture_hpp
#define Texture_hpp
#include <memory>
#include <vector>
#include <glm.hpp>
#include "Resource.h"

namespace Lutra {

    enum class TextureType
    {
        TEX2D = 0,
        RENDER,
    };

    enum class TextureFormat
    {
        RGB8 = 0,
        RGBA8,
        
        D16,
        D24,
        D32F,
        D24S8,
        D32FS8,
    };

    enum class TextureFilter
    {
        Linear,
        Nearest,
    };

    enum class TextureWrap
    {
        Clamp,
        Repeat,
    };

    class Texture : public Resource
    {
    public:
        
        Texture();
        virtual ~Texture() = default;
        
        virtual void Serialize(WriteStream& stream) override;
        virtual void Deserialize(ReadStream& stream) override;
        
        virtual uint32_t GetWidth() const = 0;
        virtual uint32_t GetHeight() const = 0;
        virtual uint32_t GetDepth() const = 0;
        
        TextureType GetType() const { return m_type; }
        
        uint32_t GetTextureID() const { return m_textureID; };
        void SetTextureID(uint32_t texID) { m_textureID = texID; }
        
        void SetFilter(TextureFilter filter) { m_filter = filter; }
        TextureFilter GetFilter() const { return m_filter; }
        
        void SetWrap(TextureWrap wrap) { m_wrap = wrap; }
        TextureWrap GetWrap() const { return m_wrap; }
        
    protected:
        
        TextureType m_type;
        TextureFilter m_filter{TextureFilter::Linear};
        TextureWrap m_wrap{TextureWrap::Clamp};
        uint32_t    m_textureID;
    };


    class Texture2D : public Texture
    {
    public:
        ~Texture2D();
        
        virtual void Serialize(WriteStream& stream) override;
        virtual void Deserialize(ReadStream& stream) override;

        virtual uint32_t GetWidth() const override { return m_width; }
        virtual uint32_t GetHeight() const override { return m_height; }
        virtual uint32_t GetDepth() const override { return 1; };
        TextureFormat GetFormat() const { return m_format; }

        void SetData(uint8_t* data, uint32_t size);
        const std::vector<uint8_t>& GetData() const { return m_data; }
        
    private:
        
        Texture2D();
        Texture2D(uint32_t width, uint32_t height, TextureFormat format);
        friend class ResourceManager;
    private:
        
        TextureFormat m_format;
        uint32_t m_width;
        uint32_t m_height;
        std::vector<uint8_t> m_data;
    };

    class RenderTexture : public Texture
    {
    public:
        ~RenderTexture();
        
        virtual void Serialize(WriteStream& stream) override;
        virtual void Deserialize(ReadStream& stream) override;

        virtual uint32_t GetWidth() const override { return m_width; }
        virtual uint32_t GetHeight() const override { return m_height; }
        virtual uint32_t GetDepth() const override { return 1; }
        
        TextureFormat GetColorFormat() const { return m_colorFormat; }
        TextureFormat GetDepthFormat() const { return m_depthFormat; }
        
        void SetClearColor(const glm::vec4& color) { m_clearColor = color; }
        const glm::vec4& GetClearColor() const { return m_clearColor; }
        
    private:
        
        RenderTexture();
        RenderTexture(uint32_t width, uint32_t height, TextureFormat colorFormat, TextureFormat depthFormat);
        friend class ResourceManager;
    private:
        
        TextureFormat m_colorFormat;
        TextureFormat m_depthFormat;
        uint32_t m_width;
        uint32_t m_height;
        glm::vec4 m_clearColor{0.0f, 0.0f, 0.0f, 1.0f};
    };
}

#endif /* Texture_hpp */
