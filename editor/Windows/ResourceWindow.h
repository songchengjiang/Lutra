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
#include "Blueprints/MaterialGraphicEditor.h"
#include "Builders/TextureBuilder.h"

namespace LutraEditor {

    class PropertyWindow;
    class IconManager;
    class ResourceWindow : public GUIWindow
    {
    public:
        ResourceWindow(const std::shared_ptr<Lutra::Scene>& scene, const std::string& root, PropertyWindow* propertyWindow);
        virtual ~ResourceWindow();
        
        virtual void Open() override;
        virtual void Close() override;
        virtual void OnGUI(uint32_t width, uint32_t height) override;
        
    private:
        
        void showMainPopup();
        
        void onResourceClicked(const std::filesystem::path& path);
        void createFolder(const std::string& folder);
        
        template<typename T, typename... Args>
        void createResource(const std::string& fileName, Args&&... args);
        void destroyResource(const std::filesystem::path& path);
        
        std::string getFolder(const std::filesystem::path& path);
        
    private:
        
        bool m_isOpen = false;
        bool m_isOpenAddResource = false;
        std::shared_ptr<Lutra::Scene>   m_scene;
        PropertyWindow* m_propertyWindow;
        ResourceBrowserWidget m_browserWidget;
        std::unique_ptr<MaterialGraphicEditor> m_materialGraphicEditor;
        std::string m_root;
        std::filesystem::path m_selectedPath;
    };

    template<typename T, typename... Args>
    void ResourceWindow::createResource(const std::string& fileName, Args&&... args)
    {
        std::string path = getFolder(m_selectedPath) + "/" + fileName;
        if (std::filesystem::exists(path))
            return;
        auto resource = T::Build(std::forward<Args>(args)...);
        resource->SetName(fileName.substr(0, fileName.find_last_of('.')));
        Lutra::ResourceManager::Instance().SaveResource(path, resource);
        Lutra::ResourceManifest::Instance().Save(m_root + "/manifest.yaml");
    }

}


#endif /* FileBrowserWindow_hpp */
