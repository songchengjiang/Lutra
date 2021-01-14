//
//  ProjectWindow.hpp
//  Editor
//
//  Created by JasonCheng on 2021/1/8.
//

#ifndef ProjectWindow_hpp
#define ProjectWindow_hpp
#include "GUIWindow.h"
#include "Lutra.h"
#include "ComponentGUI.h"

namespace LutraEditor {

    class PropertyWindow;
    class IconManager;
    class ProjectWindow : public GUIWindow
    {
    public:
        ProjectWindow(const std::shared_ptr<Lutra::Scene>& scene, PropertyWindow* propertyWindow);
        virtual ~ProjectWindow();
        
        virtual void Open() override;
        virtual void Close() override;
        virtual void OnGUI(uint32_t width, uint32_t height) override;
        
        void Reload();
        
    private:
        
        void showTree();
        void showTreeNode(Lutra::SceneObject so);
        
        void showMainPopup();
        
        Lutra::SceneObject createSceneObject(const std::string& name);
        void destroySceneObject(Lutra::SceneObject so);
        
        Lutra::SceneObject createSprite(const std::string& name);
        Lutra::SceneObject createCamera(const std::string& name);
        
        std::vector<std::shared_ptr<PropertyGUI>> getComponentGUIs(Lutra::SceneObject so);
        
    private:
        
        bool m_isOpen = false;
        PropertyWindow* m_propertyWindow;
        std::shared_ptr<Lutra::Scene> m_scene;
        std::vector<Lutra::SceneObject> m_rootSceneObjects;
        Lutra::SceneObject m_selectedSceneObject;
    };

}

#endif /* ProjectWindow_hpp */
