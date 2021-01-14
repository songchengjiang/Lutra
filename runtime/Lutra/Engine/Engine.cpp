//
//  Engine.cpp
//  Lutra
//
//  Created by JasonCheng on 2021/1/5.
//

#include "Engine.h"
#include "Renderer/Context.h"
#include "Event.h"

namespace Lutra {

    Engine::Engine(Graphic::API api)
    : m_graphic(api)
    {
        
    }

    void Engine::RemoveScene(const std::shared_ptr<Scene>& scene)
    {
        auto iter = std::find(m_sceneList.begin(), m_sceneList.end(), scene);
        if (iter != m_sceneList.end()) {
            m_sceneList.erase(iter);
        }
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

