//
//  FrameBuffer_GL.hpp
//  Lutra
//
//  Created by JasonCheng on 2021/1/7.
//

#ifndef FrameBuffer_GL_hpp
#define FrameBuffer_GL_hpp
#include "FrameBuffer.h"
#include "GLHeader.h"

namespace Lutra {

    class FrameBufferGL : public FrameBuffer
    {
    public:
        FrameBufferGL(const FramebufferSpecification& spec);
        virtual ~FrameBufferGL();

        virtual void Bind() override;
        virtual void Unbind() override;
        
        virtual const FramebufferSpecification& GetSpecification() const override { return m_spec; };

        virtual void Resize(uint32_t width, uint32_t height) override;
        
    private:
        
        void invalidate();
        
    private:
        GLuint m_id;
        FramebufferSpecification m_spec;
    };


}

#endif /* FrameBuffer_GL_hpp */
