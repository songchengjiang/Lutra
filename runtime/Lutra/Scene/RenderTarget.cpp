//
//  RenderTarget.cpp
//  Lutra
//
//  Created by JasonCheng on 2021/1/7.
//

#include "RenderTarget.h"
#include "Event.h"
#include "KeyEvent.h"
#include "MouseEvent.h"

namespace Lutra {

    RenderTarget::RenderTarget(uint32_t width, uint32_t height)
    : m_width(width)
    , m_height(height)
    {
        
    }

    RenderTexture::RenderTexture(uint32_t width, uint32_t height)
    : RenderTarget(width, height)
    {
        m_type = RenderTargetType::Texture;
    }

    RenderWindow::RenderWindow(uint32_t width, uint32_t height)
    : RenderTarget(width, height)
    {
        m_type = RenderTargetType::Window;
    }
}
