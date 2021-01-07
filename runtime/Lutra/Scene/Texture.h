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

namespace Lutra {

    enum class TextureType
    {
        TEX2D,
    };

    enum class TextureFormat
    {
        RGB8,
        RGBA8,
        
        D16,
        D24,
        D32F,
        D24S8,
        D32FS8,
    };

    class Texture
    {
    public:
        
        virtual ~Texture() = default;
        
        TextureType GetType() const { return m_type; }
        
    protected:
        
        TextureType m_type;
    };


    class Texture2D : public Texture
    {
    public:
        
        Texture2D(int width, int height, TextureFormat format);
        ~Texture2D();

        uint32_t GetWidth() const { return m_width; }
        uint32_t GetHeight() const { return m_height; }
        TextureFormat GetFormat() const { return m_format; }

        void SetData(uint8_t* data, uint32_t size);
        const std::vector<uint8_t>& GetData() const { return m_data; }
        
    private:
        
        TextureFormat m_format;
        uint32_t m_width;
        uint32_t m_height;
        std::vector<uint8_t> m_data;
    };
}

#endif /* Texture_hpp */
