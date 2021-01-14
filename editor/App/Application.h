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
        void onMenu();
        
        void newScene(const std::string& path);
        void loadScene(const std::string& path);
        void initScene(const std::string& path);
        void saveScene();
        
        void loadDefaultTemplate();
        
    private:
        
        enum class FileFlag
        {
            None = 0,
            NewScene,
            OpenScene
        };
        
        GLFWwindow* m_window;
        std::vector<std::unique_ptr<GUIWindow>> m_guiWindows;
        std::shared_ptr<Lutra::Scene> m_scene;
        Lutra::Engine m_engine;
        std::string m_projectFolder;
        
        FileFlag m_fileFlag{FileFlag::None};
    };

}

#endif /* Application_hpp */
