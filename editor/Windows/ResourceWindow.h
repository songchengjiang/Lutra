//
//  FileBrowserWindow.hpp
//  Editor
//
//  Created by JasonCheng on 2021/1/9.
//

#ifndef FileBrowserWindow_hpp
#define FileBrowserWindow_hpp
#include "GUIWindow.h"
#include <string>
#include <filesystem>

namespace LutraEditor {

    class PropertyWindow;
    class IconManager;
    class FileBrowserWindow : public GUIWindow
    {
    public:
        FileBrowserWindow(const std::string& root, PropertyWindow* propertyWindow);
        virtual ~FileBrowserWindow();
        
        virtual void Open() override;
        virtual void Close() override;
        virtual void OnGUI(uint32_t width, uint32_t height) override;
        
    private:
        
        void showPath(const std::filesystem::path& path);
        
    private:
        
        bool m_isOpen = false;
        PropertyWindow* m_propertyWindow;
        IconManager* m_iconMgr;
        std::string m_root;
        std::filesystem::path m_selectedPath;
    };

}


#endif /* FileBrowserWindow_hpp */
