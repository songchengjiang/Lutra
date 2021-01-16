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
        
        ComponentGUI(const Lutra::SceneObject& so, const std::function<void()>& updateCallback)
        : m_sceneObject(so)
        , m_callback(updateCallback)
        {}
        virtual ~ComponentGUI() = default;
        
    protected:
        
        Lutra::SceneObject    m_sceneObject;
        std::function<void()> m_callback;
    };

    class TagGUI : public ComponentGUI
    {
    public:
        
        TagGUI(const Lutra::SceneObject& so, const std::function<void()>& updateCallback)
        : ComponentGUI(so, updateCallback)
        {}
        virtual ~TagGUI() = default;
        
        virtual void OnGUI() override;
        
    };

    class TransformGUI : public ComponentGUI
    {
    public:
        
        TransformGUI(const Lutra::SceneObject& so, const std::function<void()>& updateCallback)
        : ComponentGUI(so, updateCallback)
        {}
        virtual ~TransformGUI() = default;
        
        virtual void OnGUI() override;
        
    };

    class ResourceBrowserWidget;
    class SpriteRendererGUI : public ComponentGUI
    {
    public:
        
        SpriteRendererGUI(const Lutra::SceneObject& so, const std::function<void()>& updateCallback)
        : ComponentGUI(so, updateCallback)
        {}
        virtual ~SpriteRendererGUI() = default;
        
        virtual void OnGUI() override;
        
    private:
        std::shared_ptr<ResourceBrowserWidget> m_browserWidget;
        int m_selectedMaterial = -1;
    };

    class MeshRendererGUI : public ComponentGUI
    {
    public:
        
        MeshRendererGUI(const Lutra::SceneObject& so, const std::function<void()>& updateCallback)
        : ComponentGUI(so, updateCallback)
        {}
        virtual ~MeshRendererGUI() = default;
        
        virtual void OnGUI() override;
        
    private:
        std::shared_ptr<ResourceBrowserWidget> m_browserWidget;
        int m_selectedMaterial = -1;
    };

    class MeshFilterGUI : public ComponentGUI
    {
    public:
        
        MeshFilterGUI(const Lutra::SceneObject& so, const std::function<void()>& updateCallback)
        : ComponentGUI(so, updateCallback)
        {}
        virtual ~MeshFilterGUI() = default;
        
        virtual void OnGUI() override;
        
    private:
        std::shared_ptr<ResourceBrowserWidget> m_browserWidget;
    };

    class CameraGUI : public ComponentGUI
    {
    public:
        
        CameraGUI(const Lutra::SceneObject& so, const std::function<void()>& updateCallback)
        : ComponentGUI(so, updateCallback)
        {}
        virtual ~CameraGUI() = default;
        
        virtual void OnGUI() override;
        
    private:
        std::shared_ptr<ResourceBrowserWidget> m_browserWidget;
    };

    class ComponentAppendGUI : public ComponentGUI
    {
    public:
        
        ComponentAppendGUI(const Lutra::SceneObject& so, const std::function<void()>& updateCallback)
        : ComponentGUI(so, updateCallback)
        {}
        virtual ~ComponentAppendGUI() = default;
        
        virtual void OnGUI() override;
        
    };

}


#endif /* SpriteGUI_hpp */
