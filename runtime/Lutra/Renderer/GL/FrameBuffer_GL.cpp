//
//  FrameBuffer_GL.cpp
//  Lutra
//
//  Created by JasonCheng on 2021/1/7.
//

#include "FrameBuffer_GL.h"
#include "DeviceTexture_GL.h"
#include "LutraAssert.h"

namespace Lutra {

    FrameBufferGL::FrameBufferGL(const FramebufferSpecification& spec)
    : m_spec(spec)
    , m_id(0)
    {
        invalidate();
    }

    FrameBufferGL::~FrameBufferGL()
    {
        glDeleteFramebuffers(1, &m_id);
    }

    void FrameBufferGL::Bind()
    {
        glBindFramebuffer(GL_FRAMEBUFFER, m_id);
    }

    void FrameBufferGL::Unbind()
    {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    void FrameBufferGL::Resize(uint32_t width, uint32_t height)
    {
        m_spec.Width = width;
        m_spec.Height = height;
        invalidate();
    }

    void FrameBufferGL::invalidate()
    {
        if (m_spec.ColorAttachments.empty() && m_spec.DepthStencilAttachment == nullptr)
            return;
        
        if (m_id) {
            glDeleteFramebuffers(1, &m_id);
        }

        glGenFramebuffers(1, &m_id);
        glBindFramebuffer(GL_FRAMEBUFFER, m_id);
        
        for (auto& colorAtt : m_spec.ColorAttachments) {
            DeviceTextureGL* texture = static_cast<DeviceTextureGL*>(colorAtt.get());
            texture->Bind();
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture->GetID(), 0);
        }
        
        if (m_spec.DepthStencilAttachment != nullptr) {
            DeviceTextureGL* texture = static_cast<DeviceTextureGL*>(m_spec.DepthStencilAttachment.get());
            texture->Bind();
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, texture->GetID(), 0);
        }

        LT_CORE_ASSERT(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE, "Framebuffer is incomplete!");

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        
    }

}
