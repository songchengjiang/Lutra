//
//  VertexArray_GL.cpp
//  Lutra
//
//  Created by JasonCheng on 2021/1/6.
//

#include "VertexArray_GL.h"
#include "Buffer_GL.h"

namespace Lutra {

    static GLenum ShaderDataTypeToOpenGLBaseType(BufferDataType type)
    {
        switch (type)
        {
            case BufferDataType::Float:    return GL_FLOAT;
            case BufferDataType::Float2:   return GL_FLOAT;
            case BufferDataType::Float3:   return GL_FLOAT;
            case BufferDataType::Float4:   return GL_FLOAT;
            case BufferDataType::Mat3:     return GL_FLOAT;
            case BufferDataType::Mat4:     return GL_FLOAT;
            case BufferDataType::Int:      return GL_INT;
            case BufferDataType::Int2:     return GL_INT;
            case BufferDataType::Int3:     return GL_INT;
            case BufferDataType::Int4:     return GL_INT;
            case BufferDataType::Bool:     return GL_BOOL;
            default: break;
        }

        LT_CORE_ASSERT(false, "Unknown ShaderDataType!");
        return 0;
    }

    VertexArrayGL::VertexArrayGL()
    {
        glGenVertexArrays(1, &m_id);
    }

    VertexArrayGL::~VertexArrayGL()
    {
        glDeleteVertexArrays(1, &m_id);
    }

    void VertexArrayGL::Bind() const
    {
        glBindVertexArray(m_id);
    }

    void VertexArrayGL::Unbind() const
    {
        glBindVertexArray(0);
    }

    void VertexArrayGL::AddVertexBuffer(const std::shared_ptr<VertexBuffer>& vertexBuffer)
    {
        LT_CORE_ASSERT(vertexBuffer->GetLayout().GetElements().size(), "Vertex Buffer has no layout!");

        glBindVertexArray(m_id);
        vertexBuffer->Bind();

        const auto& layout = vertexBuffer->GetLayout();
        for (const auto& element : layout)
        {
            switch (element.Type)
            {
                case BufferDataType::Float:
                case BufferDataType::Float2:
                case BufferDataType::Float3:
                case BufferDataType::Float4:
                case BufferDataType::Int:
                case BufferDataType::Int2:
                case BufferDataType::Int3:
                case BufferDataType::Int4:
                case BufferDataType::Bool:
                {
                    glEnableVertexAttribArray(m_vertexBufferIndex);
                    glVertexAttribPointer(m_vertexBufferIndex,
                        element.GetComponentCount(),
                        ShaderDataTypeToOpenGLBaseType(element.Type),
                        element.Normalized ? GL_TRUE : GL_FALSE,
                        layout.GetStride(),
                        (const void*)element.Offset);
                    m_vertexBufferIndex++;
                    break;
                }
                case BufferDataType::Mat3:
                case BufferDataType::Mat4:
                {
                    uint8_t count = element.GetComponentCount();
                    for (uint8_t i = 0; i < count; i++)
                    {
                        glEnableVertexAttribArray(m_vertexBufferIndex);
                        glVertexAttribPointer(m_vertexBufferIndex,
                            count,
                            ShaderDataTypeToOpenGLBaseType(element.Type),
                            element.Normalized ? GL_TRUE : GL_FALSE,
                            layout.GetStride(),
                            (const void*)(element.Offset + sizeof(float) * count * i));
                        glVertexAttribDivisor(m_vertexBufferIndex, 1);
                        m_vertexBufferIndex++;
                    }
                    break;
                }
                default:
                    LT_CORE_ASSERT(false, "Unknown ShaderDataType!");
            }
        }
        m_vertexBuffers.push_back(vertexBuffer);
    }

    void VertexArrayGL::SetIndexBuffer(const std::shared_ptr<IndexBuffer>& indexBuffer)
    {
        glBindVertexArray(m_id);
        indexBuffer->Bind();
        m_indexBuffer = indexBuffer;
    }

}
