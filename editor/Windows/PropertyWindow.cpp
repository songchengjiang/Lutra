//
//  PropertyWindow.cpp
//  Editor
//
//  Created by JasonCheng on 2021/1/9.
//

#include "PropertyWindow.h"

namespace LutraEditor {

    PropertyWindow::PropertyWindow()
    {
    }

    PropertyWindow::~PropertyWindow()
    {
        
    }

    void PropertyWindow::Open()
    {
        m_isOpen = true;
    }

    void PropertyWindow::Close()
    {
        m_isOpen = false;
    }

    void PropertyWindow::OnGUI(uint32_t width, uint32_t height)
    {
        ImGui::SetNextWindowSize(ImVec2(200, 600), ImGuiCond_FirstUseEver);
        ImGuiWindowFlags window_flags = ImGuiWindowFlags_None;
        ImGui::Begin("Property", &m_isOpen, window_flags);
        for (auto& property : m_propertyGUIs) {
            property->OnGUI();
        }
        ImGui::End();
    }

    void PropertyWindow::SetPropertys(const std::vector<std::shared_ptr<PropertyGUI>>& propertys)
    {
        m_propertyGUIs = propertys;
    }
}
