//
//  SceneObject.hpp
//  Lutra
//
//  Created by JasonCheng on 2021/1/4.
//

#ifndef SceneObject_hpp
#define SceneObject_hpp
#include "LutraAssert.h"
#include "entt.hpp"
#include "Scene/Scene.h"

namespace Lutra {

    class SceneObject
    {
    public:
        
        SceneObject() = default;
        SceneObject(entt::entity handle, const std::string& name, Scene *scene);
        SceneObject(const SceneObject& other) = default;
        SceneObject(const SceneObject&& other);
        
        ~SceneObject() = default;
        
        void SetName(const std::string& name) { m_name = name; }
        const std::string& GetName() const { return m_name; }
        
        template<typename T, typename... Args>
        T& AddComponent(Args&&... args)
        {
            LT_CORE_ASSERT(!HasComponent<T>(), "Entity already has component!");
            T& component = m_scene->m_registry.emplace<T>(m_handle, std::forward<Args>(args)...);
            return component;
        }

        template<typename T>
        T& GetComponent()
        {
            LT_CORE_ASSERT(HasComponent<T>(), "Entity does not have component!");
            return m_scene->m_registry.get<T>(m_handle);
        }

        template<typename T>
        bool HasComponent()
        {
            return m_scene->m_registry.has<T>(m_handle);
        }

        template<typename T>
        void RemoveComponent()
        {
            LT_CORE_ASSERT(hasComponent<T>(), "Entity does not have component!");
            m_scene->m_registry.remove<T>(m_handle);
        }

        operator bool() const { return m_handle != entt::null; }
        operator entt::entity() const { return m_handle; }
        operator uint32_t() const { return (uint32_t)m_handle; }

        bool operator==(const SceneObject& other) const
        {
            return m_handle == other.m_handle && m_scene == other.m_scene;
        }

        bool operator!=(const SceneObject& other) const
        {
            return !(*this == other);
        }
        
    private:
        entt::entity m_handle{ entt::null };
        std::string  m_name;
        Scene       *m_scene = nullptr;
    };

}

#endif /* SceneObject_hpp */
