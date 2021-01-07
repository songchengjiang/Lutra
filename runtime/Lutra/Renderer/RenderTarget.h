//
//  RenderTarget.hpp
//  Lutra
//
//  Created by JasonCheng on 2021/1/7.
//

#ifndef RenderTarget_hpp
#define RenderTarget_hpp
#include <vector>
#include <glm.hpp>

namespace Lutra {

    enum class RenderTargetType
    {
        Texture,
        Window
    };

    class Texture;
    class RenderTarget
    {
    public:
        RenderTarget(uint32_t width, uint32_t height);
        virtual ~RenderTarget() = default;
        
        RenderTargetType GetType() const { return m_type; }
        
        uint32_t GetWidth() const { return m_width; }
        uint32_t GetHeight() const { return m_height; }
        
        void SetClearColor(const glm::vec4& color) { m_clearColor = color; }
        const glm::vec4& GetClearColor() const { return m_clearColor; }
        
    protected:
        
        RenderTargetType m_type;
        uint32_t m_width, m_height;
        glm::vec4 m_clearColor{0.0f, 0.0f, 0.0f, 1.0f};
    };

    class RenderTexture : public RenderTarget
    {
    public:
        RenderTexture(uint32_t width, uint32_t height);
        virtual ~RenderTexture() = default;
        
        std::vector<std::shared_ptr<Texture>> ColorAttachments;
        std::shared_ptr<Texture> DepthStencilAttachment;
    };

    class RenderWindow : public RenderTarget
    {
    public:
        RenderWindow(uint32_t width, uint32_t height);
        virtual ~RenderWindow() = default;
        
    };

}

#endif /* RenderTarget_hpp */
