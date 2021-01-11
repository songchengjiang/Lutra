//
//  SceneObjectDelegate.hpp
//  Lutra
//
//  Created by JasonCheng on 2021/1/9.
//

#ifndef SceneObjectDelegate_hpp
#define SceneObjectDelegate_hpp

#include "Scene/SceneObject.h"

namespace Lutra {

    struct SceneObjectDelegate
    {
        SceneObject This;
        bool        IsVisible = true;
        
        void AddChild(SceneObject child);
        void RemoveChild(SceneObject child);
        std::vector<SceneObject>& GetChildren() { return m_children; }
        const std::vector<SceneObject>& GetChildren() const { return m_children; }
        
        SceneObject GetParent() const { return m_parent; }
        
        bool IsVisibleInherit() const;
        
    private:
        
        SceneObject m_parent;
        std::vector<SceneObject> m_children;
        friend class Scene;
    };
}

#endif /* SceneObjectDelegate_hpp */
