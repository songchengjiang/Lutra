//
//  GameWindow.hpp
//  Editor
//
//  Created by JasonCheng on 2021/1/13.
//

#ifndef GameWindow_hpp
#define GameWindow_hpp
#include "GUIWindow.h"
#include "Lutra.h"

namespace LutraEditor {

    class GameWindow : public GUIWindow
    {
    public:
        GameWindow(const std::shared_ptr<Lutra::Scene>& scene);
        virtual ~GameWindow();
        
        virtual void Open() override;
        virtual void Close() override;
        virtual void OnGUI(uint32_t width, uint32_t height) override;
        
    private:
        
        void resize(uint32_t width, uint32_t height);
        
    private:
        
        bool m_isOpen = false;
        std::shared_ptr<Lutra::Scene>   m_scene;
        uint32_t                        m_width,m_height;
    };

}


#endif /* GameWindow_hpp */
