//
//  RenderDevice_GL.hpp
//  Lutra
//
//  Created by JasonCheng on 2021/1/6.
//

#ifndef RenderDevice_GL_hpp
#define RenderDevice_GL_hpp

#include "Renderer/RenderDevice.h"
#include "GLHeader.h"
#include <array>

namespace Lutra {

    class RenderDeviceGL : public RenderDevice
    {
    public:
        RenderDeviceGL() = default;
        virtual ~RenderDeviceGL() = default;
        
        virtual void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) override;
        virtual void SetClearColor(const glm::vec4& color) override;
        virtual void Clear(uint8_t flag) override;
        virtual void SetGraphicState(const GraphicState& state) override;
        
        virtual std::shared_ptr<FrameBuffer> CreateFrameBuffer(const FramebufferSpecification& spec) override;
        
        virtual std::shared_ptr<VertexBuffer> CreateVertexBuffer(uint32_t size, BufferLayout layout) override;
        virtual std::shared_ptr<VertexBuffer> CreateVertexBuffer(float* vertices, uint32_t size, BufferLayout layout) override;

        virtual std::shared_ptr<IndexBuffer> CreateIndexBuffer(uint16_t* indices, uint32_t size) override;
        
        virtual std::shared_ptr<VertexArray> CreateVertexArray(const std::vector<std::shared_ptr<VertexBuffer>>& vertexBufferArray, std::shared_ptr<IndexBuffer>& indexBuffer) override;
        
        virtual std::shared_ptr<Program> CreateProgram(const std::string& vShader, const std::string& fShader) override;
        
        virtual std::shared_ptr<DeviceTexture> CreateTexture2D(int width, int height, DeviceTextureForamt format) override;

        virtual void DrawIndexed(const GraphicState& state, DevicePrimitiveType type, uint32_t elementCount) override;
        
    };

}


#endif /* RenderDevice_GL_hpp */
