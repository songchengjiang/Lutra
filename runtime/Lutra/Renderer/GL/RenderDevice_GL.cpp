//
//  RenderDevice_GL.cpp
//  Lutra
//
//  Created by JasonCheng on 2021/1/6.
//

#include "RenderDevice_GL.h"
#include "Program_GL.h"
#include "Buffer_GL.h"
#include "VertexArray_GL.h"
#include "DeviceTexture_GL.h"
#include "FrameBuffer_GL.h"

namespace Lutra {

    void RenderDeviceGL::SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
    {
        glViewport(x, y, width, height);
    }

    void RenderDeviceGL::SetClearColor(const glm::vec4& color)
    {
        glClearColor(color.r, color.g, color.b, color.a);
    }

    void RenderDeviceGL::Clear(uint8_t flag)
    {
        GLbitfield mask = 0;
        if (flag & ClearFlag::COLOR) {
            mask |= GL_COLOR_BUFFER_BIT;
        }
        if (flag & ClearFlag::DEPTH) {
            mask |= GL_DEPTH_BUFFER_BIT;
        }
        if (flag & ClearFlag::STENCIL) {
            mask |= GL_STENCIL_BUFFER_BIT;
        }
        glClear(mask);
    }

    std::shared_ptr<FrameBuffer> RenderDeviceGL::CreateFrameBuffer(const FramebufferSpecification& spec)
    {
        FrameBufferGL* frameBuffer = new FrameBufferGL(spec);
        return std::shared_ptr<FrameBuffer>(frameBuffer);
    }

    std::shared_ptr<VertexBuffer> RenderDeviceGL::CreateVertexBuffer(uint32_t size, BufferLayout layout)
    {
        VertexBufferGL* vertexBuffer = new VertexBufferGL(size);
        vertexBuffer->SetLayout(layout);
        return std::shared_ptr<VertexBuffer>{vertexBuffer};
    }

    std::shared_ptr<VertexBuffer> RenderDeviceGL::CreateVertexBuffer(float* vertices, uint32_t size, BufferLayout layout)
    {
        VertexBufferGL* vertexBuffer = new VertexBufferGL(vertices, size);
        vertexBuffer->SetLayout(layout);
        return std::shared_ptr<VertexBuffer>{vertexBuffer};
    }

    std::shared_ptr<IndexBuffer> RenderDeviceGL::CreateIndexBuffer(uint16_t* indices, uint32_t size)
    {
        return std::shared_ptr<IndexBuffer>(new IndexBufferGL(indices, size));
    }

    std::shared_ptr<VertexArray> RenderDeviceGL::CreateVertexArray(const std::vector<std::shared_ptr<VertexBuffer>>& vertexBufferArray, std::shared_ptr<IndexBuffer>& indexBuffer)
    {
        VertexArrayGL* vao = new VertexArrayGL();
        for (auto &vb : vertexBufferArray) {
            vao->AddVertexBuffer(vb);
        }
        vao->SetIndexBuffer(indexBuffer);
        return std::shared_ptr<VertexArray>(vao);
    }

    std::shared_ptr<Program> RenderDeviceGL::CreateProgram(const std::string& vShader, const std::string& fShader)
    {
        ProgramGL* program = new ProgramGL(vShader, fShader);
        return std::shared_ptr<Program>(program);
    }

    std::shared_ptr<DeviceTexture> RenderDeviceGL::CreateTexture2D(int width, int height, DeviceTextureForamt format)
    {
        DeviceTexture2DGL* texture = new DeviceTexture2DGL(width, height, format);
        return std::shared_ptr<DeviceTexture>(texture);
    }

    void RenderDeviceGL::SetGraphicState(const GraphicState& state)
    {
        if (state.BMode != GraphicBlendMode::Disabled) {
            glEnable(GL_BLEND);
        }else {
            glDisable(GL_BLEND);
        }
        switch (state.BMode) {
            case GraphicBlendMode::Normal:
                glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
                break;
            case GraphicBlendMode::Screen:
                glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_COLOR);
                break;
            case GraphicBlendMode::PremultipliedAlpha:
                glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
                break;
            case GraphicBlendMode::Add:
                glBlendFunc(GL_SRC_ALPHA, GL_ONE);
                break;
            case GraphicBlendMode::Multiply:
                glBlendFunc(GL_DST_COLOR, GL_ZERO);
                break;
            default:
                break;
        }
        
        if (state.TwoSided) {
            glDisable(GL_CULL_FACE);
        }else {
            glEnable(GL_CULL_FACE);
        }
        
        switch (state.CMode) {
            case GraphicCullMode::Front:
                glCullFace(GL_FRONT);
                break;
            case GraphicCullMode::Back:
                glCullFace(GL_BACK);
                break;
            case GraphicCullMode::FrontAndBack:
                glCullFace(GL_FRONT_AND_BACK);
                break;
        }
        
        if (state.DepthTest) {
            glEnable(GL_DEPTH_TEST);
        }else {
            glDisable(GL_DEPTH_TEST);
        }
        
        glDepthMask(state.DepthWrite? GL_TRUE: GL_FALSE);
        
        auto& colorMask = state.ColorMask;
        glColorMask(colorMask.r? GL_TRUE: GL_FALSE, colorMask.g? GL_TRUE: GL_FALSE, colorMask.b? GL_TRUE: GL_FALSE, colorMask.a? GL_TRUE: GL_FALSE);
    }

    void RenderDeviceGL::DrawIndexed(const GraphicState& state, DevicePrimitiveType type, uint32_t elementCount)
    {
        GLenum mode;
        switch (type) {
            case DevicePrimitiveType::Line:
                mode = GL_LINES;
                break;
            case DevicePrimitiveType::Triangle:
                mode = GL_TRIANGLES;
                break;
        }
        SetGraphicState(state);
        glDrawElements(mode, elementCount, GL_UNSIGNED_SHORT, nullptr);
    }
}
