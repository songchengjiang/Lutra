//
//  Scene.hpp
//  Lutra
//
//  Created by JasonCheng on 2021/1/4.
//

#ifndef Scene_hpp
#define Scene_hpp
#include <entt.hpp>
#include <string>
#include <tuple>
#include "Systems/System.h"

namespace Lutra {

    class SceneObject;
    struct FrameContext;
    class Graphic;
    class Scene
    {
    public:
        
        Scene();
        ~Scene() = default;
        
        SceneObject CreateSceneObject(const std::string& name = std::string());
        void DestroySceneObject(SceneObject so);
        
        template<typename T>
        void AppendSystem();
        
        void OnUpdate(const FrameContext& context, Graphic& graphic);
        
    private:
        
    private:
        
        entt::registry m_registry;
        std::vector<std::unique_ptr<System>> m_systemList;
        friend class SceneObject;
        friend class System;
    };

    template<typename T>
    void Scene::AppendSystem()
    {
        m_systemList.emplace_back(new T(this));
    }

}

#endif /* Scene_hpp */
