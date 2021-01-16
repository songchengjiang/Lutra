//
//  ResourceBrowserWindow.hpp
//  Editor
//
//  Created by JasonCheng on 2021/1/13.
//

#ifndef ResourceBrowserWindow_hpp
#define ResourceBrowserWindow_hpp
#include <unordered_map>
#include <filesystem>
#include <functional>
#include "GUIWindow.h"
#include "Lutra.h"
#include "IconManager.h"

namespace LutraEditor {

    struct ExampleAppLog;
    class ResourceBrowserWidget
    {
    public:
        ResourceBrowserWidget(const std::string& root, const std::unordered_map<std::string, IconType>& exts, bool showFolder = true);
        virtual ~ResourceBrowserWidget();
        
        bool OnGUI();
        
        void SetClickItemCallback(const std::function<void(const std::filesystem::path& path)>& func) { m_callBack = func; }
        
        std::string GetSelectedPath() const { return m_selectedPath; }
        void Reset() { m_selectedPath = m_root; }
        
    private:
        
        bool showPath(const std::filesystem::path& path);
        
    private:
        
        std::string m_root;
        std::filesystem::path m_selectedPath;
        std::unordered_map<std::string, IconType> m_exts;
        bool m_showFolder;
        
        std::function<void(const std::filesystem::path& path)> m_callBack;
    };

}

#endif /* ResourceBrowserWindow_hpp */
