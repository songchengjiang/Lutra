//
//  PropertyWindow.hpp
//  Editor
//
//  Created by JasonCheng on 2021/1/9.
//

#ifndef PropertyWindow_hpp
#define PropertyWindow_hpp
#include "GUIWindow.h"
#include "PropertyGUI.h"
#include <memory>
#include <vector>

namespace LutraEditor {

    class PropertyWindow : public GUIWindow
    {
    public:
        PropertyWindow();
        virtual ~PropertyWindow();
        
        virtual void Open() override;
        virtual void Close() override;
        virtual void OnGUI(uint32_t width, uint32_t height) override;
        
        void SetPropertys(const std::vector<std::shared_ptr<PropertyGUI>>& propertys);
        void Clear() { m_propertyGUIs.clear(); }
        
    private:
        
        bool m_isOpen = false;
        std::vector<std::shared_ptr<PropertyGUI>> m_propertyGUIs;
    };

}

#endif /* PropertyWindow_hpp */
