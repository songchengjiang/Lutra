//
//  Engine.cpp
//  Lutra
//
//  Created by JasonCheng on 2021/1/5.
//

#include "Engine.h"
#include "Renderer/Context.h"

namespace Lutra {

    Engine::Engine()
    : m_graphic(Graphic::API::OpenGL)
    {
        
    }

    void Engine::OnUpdateFrame(Timestep time)
    {
        m_graphic.Begin();
        FrameContext context{time};
        for (auto& scene : m_sceneList) {
            scene->OnUpdate(context, m_graphic);
        }
        m_graphic.End();
    }
}

