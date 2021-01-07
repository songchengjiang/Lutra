//
//  System.cpp
//  Lutra
//
//  Created by JasonCheng on 2021/1/5.
//

#include "System.h"
#include "Scene.h"

namespace Lutra {
        
    System::System(Scene* scene)
    : m_scene(scene)
    {
        
    }

    System::~System()
    {
        
    }

    entt::registry& System::registry()
    {
        return m_scene->m_registry;
    }

}
