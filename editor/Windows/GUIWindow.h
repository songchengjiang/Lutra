//
//  GUIWindow.hpp
//  Editor
//
//  Created by JasonCheng on 2021/1/8.
//

#ifndef GUIWindow_hpp
#define GUIWindow_hpp
#include <imgui.h>

namespace LutraEditor {

    class GUIWindow
    {
    public:
        GUIWindow() = default;
        virtual ~GUIWindow() = default;
        
        virtual void Open() = 0;
        virtual void Close() = 0;
        virtual void OnGUI(uint32_t width, uint32_t height) = 0;
    };

}

#endif /* GUIWindow_hpp */
