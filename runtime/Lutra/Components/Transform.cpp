//
//  Transform.cpp
//  Lutra
//
//  Created by JasonCheng on 2021/1/4.
//

#include "Transform.h"

namespace Lutra {

    void Transform::AddChild(Transform* child)
    {
        child->m_parent = this;
        m_children.push_back(child);
    }

    void Transform::RemoveChild(Transform* child)
    {
        child->m_parent = nullptr;
        m_children.erase(std::find(m_children.begin(), m_children.end(), child));
    }
}
