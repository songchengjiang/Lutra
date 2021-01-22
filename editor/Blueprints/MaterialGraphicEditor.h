//
//  MaterialGraphicEditor.hpp
//  Editor
//
//  Created by JasonCheng on 2021/1/18.
//

#ifndef MaterialGraphicEditor_hpp
#define MaterialGraphicEditor_hpp
#include "GUIWindow.h"
#include <Lutra.h>
#include "Blueprint.h"
#include "CameraManipulator.h"
#include <functional>

namespace LutraEditor {

    class MaterialGraphicEditor
    {
    public:
        MaterialGraphicEditor(const std::shared_ptr<Lutra::Scene>& scene, const std::shared_ptr<Lutra::Material>& material);
        ~MaterialGraphicEditor();
        
        void Open();
        void Close();
        void OnGUI(uint32_t width, uint32_t height);
        
    private:
        
        bool m_isOpen = false;
        std::shared_ptr<Lutra::Scene> m_scene;
        std::shared_ptr<Lutra::Material> m_material;
        std::shared_ptr<Lutra::RenderTexture> m_colorTexture;
        std::unique_ptr<CameraManipulator> m_cameraManipulator;
        Lutra::SceneObject m_meshSO;
        Lutra::SceneObject m_cameraSO;
        Blueprint m_blueprint;
    };

}

#endif /* MaterialGraphicEditor_hpp */
