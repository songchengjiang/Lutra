//
//  Buffer_GL.cpp
//  Lutra
//
//  Created by JasonCheng on 2021/1/6.
//

#include "Buffer_GL.h"

namespace Lutra {

    VertexBufferGL::VertexBufferGL(uint32_t size)
    {
        glGenBuffers(1, &m_id);
        glBindBuffer(GL_ARRAY_BUFFER, m_id);
        glBufferData(GL_ARRAY_BUFFER, size, nullptr, GL_DYNAMIC_DRAW);
    }

    VertexBufferGL::VertexBufferGL(float* vertices, uint32_t size)
    {
        glGenBuffers(1, &m_id);
        glBindBuffer(GL_ARRAY_BUFFER, m_id);
        glBufferData(GL_ARRAY_BUFFER, size * sizeof(float), vertices, GL_STATIC_DRAW);
    }

    VertexBufferGL::~VertexBufferGL()
    {
        glDeleteBuffers(1, &m_id);
    }

    void VertexBufferGL::Bind()
    {
        glBindBuffer(GL_ARRAY_BUFFER, m_id);
        UpdateUsage();
    }

    void VertexBufferGL::Unbind()
    {
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

    void VertexBufferGL::SetData(float* vertices, uint32_t size)
    {
        glBindBuffer(GL_ARRAY_BUFFER, m_id);
        glBufferSubData(GL_ARRAY_BUFFER, 0, size * sizeof(float), vertices);
    }


    IndexBufferGL::IndexBufferGL(uint16_t* indices, uint32_t size)
    : m_size(size)
    {
        glGenBuffers(1, &m_id);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_id);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, size * sizeof(uint16_t), indices, GL_STATIC_DRAW);
    }

    IndexBufferGL::~IndexBufferGL()
    {
        glDeleteBuffers(1, &m_id);
    }

    void IndexBufferGL::Bind()
    {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_id);
        UpdateUsage();
    }

    void IndexBufferGL::Unbind()
    {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }

}

