//
//  SpriteGUI.hpp
//  Editor
//
//  Created by JasonCheng on 2021/1/8.
//

#ifndef SpriteGUI_hpp
#define SpriteGUI_hpp
#include <imgui.h>
#include "Lutra.h"
#include "PropertyGUI.h"

namespace LutraEditor {

    class ComponentGUI : public PropertyGUI
    {
    public:
        
        ComponentGUI(const Lutra::SceneObject& so)
        : m_sceneObject(so)
        {}
        virtual ~ComponentGUI() = default;
        
    protected:
        
        Lutra::SceneObject m_sceneObject;
    };

    class TagGUI : public ComponentGUI
    {
    public:
        
        TagGUI(const Lutra::SceneObject& so)
        : ComponentGUI(so)
        {}
        virtual ~TagGUI() = default;
        
        virtual void OnGUI() override;
        
    };

    class TransformGUI : public ComponentGUI
    {
    public:
        
        TransformGUI(const Lutra::SceneObject& so)
        : ComponentGUI(so)
        {}
        virtual ~TransformGUI() = default;
        
        virtual void OnGUI() override;
        
    };

    class SpriteRendererGUI : public ComponentGUI
    {
    public:
        
        SpriteRendererGUI(const Lutra::SceneObject& so)
        : ComponentGUI(so)
        {}
        virtual ~SpriteRendererGUI() = default;
        
        virtual void OnGUI() override;
    };

    class MeshFilterGUI : public ComponentGUI
    {
    public:
        
        MeshFilterGUI(const Lutra::SceneObject& so)
        : ComponentGUI(so)
        {}
        virtual ~MeshFilterGUI() = default;
        
        virtual void OnGUI() override;
    };

    class CameraGUI : public ComponentGUI
    {
    public:
        
        CameraGUI(const Lutra::SceneObject& so)
        : ComponentGUI(so)
        {}
        virtual ~CameraGUI() = default;
        
        virtual void OnGUI() override;
    };

    class ComponentAppendGUI : public ComponentGUI
    {
    public:
        
        ComponentAppendGUI(const Lutra::SceneObject& so, const std::function<void()>& updateCallback)
        : ComponentGUI(so)
        , m_callback(updateCallback)
        {}
        virtual ~ComponentAppendGUI() = default;
        
        virtual void OnGUI() override;
        
    private:
        std::function<void()> m_callback;
    };

}


#endif /* SpriteGUI_hpp */
