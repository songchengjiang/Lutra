//
//  SceneObjectDelegate.cpp
//  Lutra
//
//  Created by JasonCheng on 2021/1/9.
//

#include "SceneObjectDelegate.h"

namespace Lutra {

    void SceneObjectDelegate::AddChild(SceneObject child)
    {
        child.GetComponent<SceneObjectDelegate>().m_parent = This;
        m_children.push_back(child);
    }

    void SceneObjectDelegate::RemoveChild(SceneObject child)
    {
        child.GetComponent<SceneObjectDelegate>().m_parent = SceneObject();
        m_children.erase(std::find(m_children.begin(), m_children.end(), child));
    }

    static bool IsVisibleInheritCheck(const SceneObjectDelegate& sod)
    {
        if (!sod.GetParent()) {
            return sod.IsVisible;
        }
        return IsVisibleInheritCheck(sod.GetParent().GetComponent<SceneObjectDelegate>()) & sod.IsVisible;
    }

    bool SceneObjectDelegate::IsVisibleInherit() const
    {
        return IsVisibleInheritCheck(*this);
    }
}
