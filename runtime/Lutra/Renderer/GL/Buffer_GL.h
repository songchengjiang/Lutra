//
//  Buffer_GL.hpp
//  Lutra
//
//  Created by JasonCheng on 2021/1/6.
//

#ifndef Buffer_GL_hpp
#define Buffer_GL_hpp

#include "Renderer/Buffer.h"
#include "GLHeader.h"

namespace Lutra {

    class VertexBufferGL : public VertexBuffer
    {
    public:
        VertexBufferGL(uint32_t size);
        VertexBufferGL(float* vertices, uint32_t size);
        virtual ~VertexBufferGL();

        virtual void Bind() const override;
        virtual void Unbind() const override;
        
        virtual void SetData(float* vertices, uint32_t size) override;

        virtual const BufferLayout& GetLayout() const override { return m_layout; }
        virtual void SetLayout(const BufferLayout& layout) override { m_layout = layout; }

    private:
        
        GLuint m_id;
        BufferLayout m_layout;
    };

    class IndexBufferGL : public IndexBuffer
    {
    public:
        
        IndexBufferGL(uint16_t* indices, uint32_t size);
        virtual ~IndexBufferGL();

        virtual void Bind() const override;
        virtual void Unbind() const override;
        
        virtual uint32_t GetSize() const override { return m_size;}
        
    private:
        
        GLuint m_id;
        uint32_t m_size;
    };

}

#endif /* Buffer_GL_hpp */
