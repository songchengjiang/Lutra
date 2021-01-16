//
//  RenderDevice.hpp
//  Lutra
//
//  Created by JasonCheng on 2021/1/6.
//

#ifndef RenderDevice_hpp
#define RenderDevice_hpp
#include "Renderer/Buffer.h"
#include "Renderer/FrameBuffer.h"
#include "Renderer/DeviceTexture.h"
#include <glm.hpp>
#include <memory>

namespace Lutra {

    class VertexBuffer;
    class IndexBuffer;
    class VertexArray;
    class Program;

    enum ClearFlag
    {
        COLOR   = 1,
        DEPTH   = 1<<1,
        STENCIL = 1<<2
    };

    enum class GraphicBlendMode
    {
        Disabled,
        Normal,
        Screen,
        PremultipliedAlpha,
        Add,
        Multiply
    };

    enum class GraphicCullMode
    {
        Front,
        Back,
        FrontAndBack
    };

    enum class DevicePrimitiveType
    {
        Line = 0,
        Triangle
    };

    struct GraphicState
    {
        GraphicBlendMode BMode;
        GraphicCullMode CMode;
        bool        DepthTest;
        bool        DepthWrite;
        bool        TwoSided;
        glm::bvec4  ColorMask;
    };

    class RenderDevice
    {
    public:
        
        RenderDevice() = default;
        virtual ~RenderDevice() = default;
        
        virtual void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) = 0;
        virtual void SetClearColor(const glm::vec4& color) = 0;
        virtual void Clear(uint8_t flag) = 0;
        virtual void SetGraphicState(const GraphicState& state) = 0;
        
        virtual std::shared_ptr<FrameBuffer> CreateFrameBuffer(const FramebufferSpecification&) = 0;
        
        virtual std::shared_ptr<VertexBuffer> CreateVertexBuffer(uint32_t size, BufferLayout layout) = 0;
        virtual std::shared_ptr<VertexBuffer> CreateVertexBuffer(float* vertices, uint32_t size, BufferLayout layout) = 0;

        virtual std::shared_ptr<IndexBuffer> CreateIndexBuffer(uint16_t* indices, uint32_t size) = 0;
        
        virtual std::shared_ptr<VertexArray> CreateVertexArray(const std::vector<std::shared_ptr<VertexBuffer>>& vertexBufferArray, std::shared_ptr<IndexBuffer>& indexBuffer) = 0;
        
        virtual std::shared_ptr<Program> CreateProgram(const std::string& vShader, const std::string& fShader) = 0;
        
        virtual std::shared_ptr<DeviceTexture> CreateTexture2D(int width, int height, DeviceTextureForamt format) = 0;
        
        virtual void DrawIndexed(const GraphicState& state, DevicePrimitiveType type, uint32_t elementCount) = 0;
    

    };

}


#endif /* RenderDevice_hpp */
