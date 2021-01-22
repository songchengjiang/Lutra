//
//  CameraManipulator.cpp
//  Editor
//
//  Created by JasonCheng on 2021/1/14.
//

#include "CameraManipulator.h"
#include <imgui.h>
#include "Utils/MathHelper.h"

namespace LutraEditor {

    static glm::vec2 UnitMouseCoordinate(ImVec2 coord, glm::vec2 size)
    {
        return {coord.x / size.x * 2.0f - 1.0f, (1.0 - coord.y / size.y) * 2.0f - 1.0f};
    }

    CameraManipulator::CameraManipulator(Lutra::SceneObject cameraSO, const glm::vec3& initPos, const glm::vec3& initRot, float zoomSpeed)
    : m_cameraSO(cameraSO)
    , m_zoomSpeed(zoomSpeed)
    , m_rotation(initRot)
    , m_center(glm::vec3(0.0f))
    , m_translate(initPos)
    {
        updateCamera();
    }

    void CameraManipulator::Update(uint32_t width, uint32_t height)
    {
        if (!ImGui::IsWindowHovered())
            return;
        if (ImGui::IsMouseDown(ImGuiMouseButton_Right)) {
            m_g0 = m_g1 = UnitMouseCoordinate(ImGui::GetMousePos(), {width, height});
        }
        if (ImGui::IsMouseDragging(ImGuiMouseButton_Right)) {
            ImVec2 delta = ImGui::GetIO().MouseDelta;
            m_g0 = UnitMouseCoordinate({ImGui::GetMousePos().x - delta.x, ImGui::GetMousePos().y - delta.y}, {width, height});
            m_g1 = UnitMouseCoordinate({ImGui::GetMousePos().x, ImGui::GetMousePos().y}, {width, height});
            //printf("delta=(%f, %f)\n", delta.x, delta.y);
            rotateCamera(m_g0, m_g1);
        }
        
        zoomCamera(-ImGui::GetIO().MouseWheel);
    }

    void CameraManipulator::updateCamera()
    {
        glm::mat4 result = glm::inverse(glm::lookAt(glm::quat(glm::radians(m_rotation)) * m_translate, m_center, glm::vec3(0.0f, 1.0f, 0.0f)));

        glm::vec3 scale;
        glm::quat rotation;
        glm::vec3 translation;
        MathHelper::decompose(result, &translation, &rotation, &scale);
        
        m_cameraSO.GetComponent<Lutra::Transform>().Position = translation;
        m_cameraSO.GetComponent<Lutra::Transform>().Rotation = glm::degrees(glm::eulerAngles(rotation));
    }

    void CameraManipulator::rotateCamera(const glm::vec2 &g0, const glm::vec2 &g1)
    {
        if (g0 == g1) return;
        //float angle;
        //glm::vec3 axes;
        //trackball(axes, angle, g0.x, g0.y, g1.x, g1.y);
        //m_rotation = glm::angleAxis(angle * 2.0f, axes) * m_rotation;
        m_rotation += glm::vec3(g1.y - g0.y, g0.x - g1.x, 0.0f) * 100.0f;
        m_rotation.x = std::clamp(m_rotation.x, -89.9f, 89.9f);
        updateCamera();
    }

    void CameraManipulator::moveCamera(const glm::vec2 &g0, const glm::vec2 &g1)
    {
        if (g0 == g1) return;
        glm::vec2 delta = g0 - g1;
        delta *= 10;
        m_center += glm::quat(glm::radians(m_rotation)) * glm::vec3(delta.x, delta.y, .0f);
        updateCamera();
    }

    void CameraManipulator::zoomCamera(float zoomDelta)
    {
        m_translate.z += zoomDelta * 0.1f * std::max(0.1f, m_translate.z);
        m_translate.z = std::max(0.1f, m_translate.z);
        updateCamera();
    }
//
//    void CameraManipulator::trackball(glm::vec3& axis, float& angle, float p1x, float p1y, float p2x, float p2y)
//    {
//        float trackballSize = 1.0f;
//
//        glm::vec3 uv = m_rotation * glm::vec3(0.0f, 1.0f, 0.0f);
//        glm::vec3 sv = m_rotation * glm::vec3(1.0f, 0.0f, 0.0f);
//        glm::vec3 lv = m_rotation * glm::vec3(0.0f, 0.0f, -1.0f);
//
//        glm::vec3 p1 = sv * p1x + uv * p1y - lv * tb_project_to_sphere(trackballSize, p1x, p1y);
//        glm::vec3 p2 = sv * p2x + uv * p2y - lv * tb_project_to_sphere(trackballSize, p2x, p2y);
//
//        axis = glm::cross(p2, p1);
//        axis = glm::normalize(axis);
//
//        float t = glm::length(p2 - p1) / (2.0f * trackballSize);
//
//        if (t > 1.0f) t = 1.0f;
//        if (t < -1.0f) t = -1.0f;
//        angle = asin(t);
//    }
//
//    float CameraManipulator::tb_project_to_sphere(float r, float x, float y)
//    {
//        float d, t, z;
//        d = sqrt(x*x + y*y);
//        if (d < r * 0.70710678118654752440f)
//        {
//            z = sqrt(r*r - d*d);
//        }
//        else
//        {
//            t = r / 1.41421356237309504880f;
//            z = t*t / d;
//        }
//        return z;
//    }
}
