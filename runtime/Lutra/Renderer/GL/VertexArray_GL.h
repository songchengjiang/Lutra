//
//  VertexArray_GL.hpp
//  Lutra
//
//  Created by JasonCheng on 2021/1/6.
//

#ifndef VertexArray_GL_hpp
#define VertexArray_GL_hpp
#include "VertexArray.h"
#include <vector>
#include "GLHeader.h"

namespace Lutra {

    class VertexArrayGL : public VertexArray
    {
    public:
        VertexArrayGL();
        virtual ~VertexArrayGL();

        virtual void Bind(const std::shared_ptr<Program>& program) override;
        virtual void Unbind() override;

        virtual void AddVertexBuffer(const std::shared_ptr<VertexBuffer>& vertexBuffer) override;
        virtual void SetIndexBuffer(const std::shared_ptr<IndexBuffer>& indexBuffer) override;

        virtual const std::vector<std::shared_ptr<VertexBuffer>>& GetVertexBuffers() const override { return m_vertexBuffers; };
        virtual const std::shared_ptr<IndexBuffer>& GetIndexBuffer() const override { return m_indexBuffer; }

    private:
        void bindVertexAttributes();
        
    private:
        
        GLuint m_id;
        std::vector<std::shared_ptr<VertexBuffer>> m_vertexBuffers;
        std::shared_ptr<IndexBuffer> m_indexBuffer;
        std::shared_ptr<Program> m_bindingProgram;
    };

}

#endif /* VertexArray_GL_hpp */
