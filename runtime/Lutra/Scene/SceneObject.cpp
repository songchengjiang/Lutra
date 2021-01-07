//
//  SceneObject.cpp
//  Lutra
//
//  Created by JasonCheng on 2021/1/4.
//

#include "SceneObject.h"

namespace Lutra {

    SceneObject::SceneObject(entt::entity handle, const std::string& name, Scene *scene)
        : m_handle(handle)
        , m_name(name)
        , m_scene(scene)
    {
        
    }

    SceneObject::SceneObject(const SceneObject&& other)
        : m_handle(std::move(other.m_handle))
        , m_scene(std::move(other.m_scene))
    {
        
    }

}
