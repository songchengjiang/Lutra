//
//  ResourceGUI.hpp
//  Editor
//
//  Created by JasonCheng on 2021/1/12.
//

#ifndef ResourceGUI_hpp
#define ResourceGUI_hpp
#include <functional>
#include <imgui.h>
#include "Lutra.h"
#include "PropertyGUI.h"

namespace LutraEditor {

    class ResourceGUI : public PropertyGUI
    {
    public:
        
        ResourceGUI() = default;
        virtual ~ResourceGUI() = default;
        
        void SetResourceChangedCallback(const std::function<void()>& func) { m_changedCallback = func;}
        
    protected:
        
        std::function<void()> m_changedCallback;
    };

    class TextureGUI : public ResourceGUI
    {
    public:
        
        TextureGUI(const std::shared_ptr<Lutra::Texture>& tex);
        virtual ~TextureGUI() = default;
        
        virtual void OnGUI() override;
        
    private:
        
        std::shared_ptr<Lutra::Texture> m_texture;
        uint32_t m_texID;
    };

    class MeshGUI : public ResourceGUI
    {
    public:
        
        MeshGUI(const std::shared_ptr<Lutra::Mesh>& mesh)
        : m_mesh(mesh)
        {}
        virtual ~MeshGUI() = default;
        
        virtual void OnGUI() override;
        
    private:
        
        std::shared_ptr<Lutra::Mesh> m_mesh;
    };

    class ResourceBrowserWidget;
    class MaterialGUI : public ResourceGUI
    {
    public:
        
        MaterialGUI(const std::shared_ptr<Lutra::Material>& material)
        : m_material(material)
        {}
        virtual ~MaterialGUI() = default;
        
        virtual void OnGUI() override;
        
    private:
        
        std::shared_ptr<Lutra::Material> m_material;
        std::shared_ptr<ResourceBrowserWidget> m_browserWidget;
    };
}

#endif /* ResourceGUI_hpp */
