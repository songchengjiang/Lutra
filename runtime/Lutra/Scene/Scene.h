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
#include "Events/Event.h"

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
        const std::vector<SceneObject>& GetSceneObjects() const { return m_sceneObjectList; }
        
        EventDispatcher& GetEventDispatcher() { return m_eventDispatcher; }
        
        template<typename T>
        void AppendSystem();
        const std::vector<std::unique_ptr<System>>& GetSystems() const { return m_systemList; }
        
        void OnUpdate(const FrameContext& context, Graphic& graphic);
        
    private:
        
    private:
        
        entt::registry                       m_registry;
        std::vector<SceneObject>             m_sceneObjectList;
        std::vector<std::unique_ptr<System>> m_systemList;
        EventDispatcher                      m_eventDispatcher;
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
