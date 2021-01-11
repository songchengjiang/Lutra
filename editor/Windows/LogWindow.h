//
//  LogWindow.hpp
//  Editor
//
//  Created by JasonCheng on 2021/1/9.
//

#ifndef LogWindow_hpp
#define LogWindow_hpp
#include "GUIWindow.h"
#include "Lutra.h"

namespace LutraEditor {

    struct ExampleAppLog;
    class LogWindow : public GUIWindow
    {
    public:
        LogWindow(const std::shared_ptr<Lutra::Scene>& scene);
        virtual ~LogWindow();
        
        virtual void Open() override;
        virtual void Close() override;
        virtual void OnGUI(uint32_t width, uint32_t height) override;
        
        void AddLog(const std::string& log);
        
    private:
        
        bool m_isOpen = false;
        std::shared_ptr<Lutra::Scene> m_scene;
        ExampleAppLog* m_logImp;
    };

}


#endif /* LogWindow_hpp */
