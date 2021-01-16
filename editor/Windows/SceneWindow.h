//
//  SceneWindow.hpp
//  Editor
//
//  Created by JasonCheng on 2021/1/8.
//

#ifndef SceneWindow_hpp
#define SceneWindow_hpp
#include "GUIWindow.h"
#include "Lutra.h"
#include "CameraManipulator.h"

namespace LutraEditor {

    class SceneWindow : public GUIWindow
    {
    public:
        SceneWindow(const std::shared_ptr<Lutra::Scene>& scene);
        virtual ~SceneWindow();
        
        virtual void Open() override;
        virtual void Close() override;
        virtual void OnGUI(uint32_t width, uint32_t height) override;
        
        void OnNotifySceneObjectAdded(Lutra::SceneObject so);
        void OnNotifySceneObjectPropoertyChanged(Lutra::SceneObject so);
        void OnNotifySceneObjectRemoved(Lutra::SceneObject so);
        
    private:
        
        void resize(uint32_t width, uint32_t height);
        void createReferenceGrid();
        
    private:
        
        bool m_isOpen = false;
        std::shared_ptr<Lutra::Scene>   m_scene;
        std::shared_ptr<Lutra::RenderTexture> m_colorTexture;
        std::unique_ptr<CameraManipulator> m_cameraManipulator;
        std::unordered_map<uint32_t, Lutra::SceneObject> m_cameraMap;
        Lutra::SceneObject              m_sceneCamera;
        uint32_t                        m_width,m_height;
    };

}

#endif /* SceneWindow_hpp */
