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
#include <unordered_map>
#include <filesystem>
#include "IconManager.h"
#include "ResourceBrowserWidget.h"

namespace LutraEditor {

    class PropertyWindow;
    class IconManager;
    class ResourceWindow : public GUIWindow
    {
    public:
        ResourceWindow(const std::string& root, PropertyWindow* propertyWindow);
        virtual ~ResourceWindow();
        
        virtual void Open() override;
        virtual void Close() override;
        virtual void OnGUI(uint32_t width, uint32_t height) override;
        
    private:
        
        void showMainPopup();
        
        void onResourceClicked(const std::filesystem::path& path);
        void createFolder(const std::string& folder);
        void createImageTexture(const std::string& name, const std::string& imagePath);
        void createRenderTexture(const std::string& name, uint32_t width, uint32_t height);
        void createPlaneMesh(const std::string& name);
        void createUnlitMaterial(const std::string& name);
        
        void destroyResource(const std::filesystem::path& path);
        
        std::string getFolder(const std::filesystem::path& path);
        
    private:
        
        bool m_isOpen = false;
        bool m_isOpenAddResource = false;
        PropertyWindow* m_propertyWindow;
        ResourceBrowserWidget m_browserWidget;
        std::string m_root;
        std::filesystem::path m_selectedPath;
    };

}


#endif /* FileBrowserWindow_hpp */
