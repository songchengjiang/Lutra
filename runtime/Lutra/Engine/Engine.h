//
//  Engine.hpp
//  Lutra
//
//  Created by JasonCheng on 2021/1/5.
//

#ifndef Engine_hpp
#define Engine_hpp
#include "Scene/Timestep.h"
#include "Scene/Scene.h"
#include "Renderer/Graphic.h"

namespace Lutra {

    class Engine
    {
    public:
        Engine(Graphic::API api = Graphic::API::OpenGL);
        ~Engine() = default;
        
        void AddScene(const std::shared_ptr<Scene>& scene) { m_sceneList.push_back(scene); }
        const std::shared_ptr<Scene>& GetScene(uint32_t idx) const { return m_sceneList[idx];}
        void RemoveScene(const std::shared_ptr<Scene>& scene);
        
        void OnUpdateFrame(Timestep time);
        
    private:
        
        std::vector<std::shared_ptr<Scene>> m_sceneList;
        Graphic m_graphic;
    };
}

#endif /* Engine_hpp */
