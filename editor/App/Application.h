//
//  Application.hpp
//  Editor
//
//  Created by JasonCheng on 2021/1/8.
//

#ifndef Application_hpp
#define Application_hpp
#include "Lutra.h"
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include "GUIWindow.h"
#include "IconManager.h"

namespace LutraEditor {

    class Application
    {
    public:
        Application();
        ~Application();
        
        void Run();
        
    private:
        
        void onInit();
        void onUpdate(double frameTime);
        void onGUIInit();
        void onGUIUpdate();
        
    private:
        
        GLFWwindow* m_window;
        std::vector<std::unique_ptr<GUIWindow>> m_guiWindows;
        Lutra::Engine m_engine;
    };

}

#endif /* Application_hpp */
