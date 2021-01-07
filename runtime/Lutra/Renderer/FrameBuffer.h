//
//  Framebuffer.hpp
//  Lutra
//
//  Created by JasonCheng on 2021/1/7.
//

#ifndef Framebuffer_hpp
#define Framebuffer_hpp
#include <vector>

namespace Lutra {
    
    class DeviceTexture;
    struct FramebufferSpecification
    {
        uint32_t Width;
        uint32_t Height;
        std::vector<std::shared_ptr<DeviceTexture>> ColorAttachments;
        std::shared_ptr<DeviceTexture> DepthStencilAttachment;
    };

    class FrameBuffer
    {
    public:
        virtual ~FrameBuffer() = default;

        virtual void Bind() = 0;
        virtual void Unbind() = 0;
        
        virtual const FramebufferSpecification& GetSpecification() const = 0;

        virtual void Resize(uint32_t width, uint32_t height) = 0;
    };


}

#endif /* Framebuffer_hpp */
