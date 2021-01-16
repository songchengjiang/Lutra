//
//  CameraManipulator.hpp
//  Editor
//
//  Created by JasonCheng on 2021/1/14.
//

#ifndef CameraManipulator_hpp
#define CameraManipulator_hpp

#include "Lutra.h"

namespace LutraEditor {

    class CameraManipulator
    {
    public:
        CameraManipulator(Lutra::SceneObject cameraSO, const glm::vec3& initPos, const glm::vec3& initRot, float zoomSpeed = 1.0f);
        virtual ~CameraManipulator() = default;
        
        void Update(uint32_t width, uint32_t height);
        
    private:
        
        void updateCamera();
        void rotateCamera(const glm::vec2& g0, const glm::vec2& g1);
        void moveCamera(const glm::vec2& g0, const glm::vec2& g1);
        void zoomCamera(float zoomDelta);
        
//        void trackball(glm::vec3& axis, float & angle, float p1x, float p1y, float p2x, float p2y);
//        float tb_project_to_sphere(float r, float x, float y);
        
    private:
        
        Lutra::SceneObject m_cameraSO;
        
        glm::vec2 m_g0;
        glm::vec2 m_g1;
        
        glm::vec3 m_rotation;
        glm::vec3 m_center;
        glm::vec3 m_translate;
        
        float m_zoomSpeed = 1.0f;
    };

}

#endif /* CameraManipulator_hpp */
