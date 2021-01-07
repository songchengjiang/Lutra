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

    void RenderDeviceGL::DrawIndexed(const std::shared_ptr<VertexArray>& vao)
    {
        vao->Bind();
        glDrawElements(GL_TRIANGLES, vao->GetIndexBuffer()->GetSize(), GL_UNSIGNED_SHORT, nullptr);
    }
}
