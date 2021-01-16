//
//  ResourceBrowserWindow.cpp
//  Editor
//
//  Created by JasonCheng on 2021/1/13.
//

#include "ResourceBrowserWidget.h"

namespace LutraEditor {

    ResourceBrowserWidget::ResourceBrowserWidget(const std::string& root, const std::unordered_map<std::string, IconType>& exts, bool showFolder)
    : m_root(root)
    , m_exts(exts)
    , m_showFolder(showFolder)
    {
        m_selectedPath = m_root;
    }

    ResourceBrowserWidget::~ResourceBrowserWidget()
    {
    }

    bool ResourceBrowserWidget::OnGUI()
    {
        bool isItemClicked = false;
        if (std::filesystem::is_directory(m_root)) {
            std::filesystem::directory_iterator end_iter;
            for (std::filesystem::directory_iterator iter(m_root); iter != end_iter; ++iter) {
                isItemClicked |= showPath(*iter);
            }
        }
        
        return isItemClicked;
    }

    bool ResourceBrowserWidget::showPath(const std::filesystem::path& path)
    {
        if (!std::filesystem::exists(path))
            return false;
        
        ImGuiTreeNodeFlags node_flags = ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_SpanFullWidth;
        if (m_selectedPath == path)
            node_flags |= ImGuiTreeNodeFlags_Selected;
        
        IconType iconType = IconType::Folder;
        bool isDirectory = std::filesystem::is_directory(path);
        if (!isDirectory) {
            node_flags |= ImGuiTreeNodeFlags_Leaf;
            if (m_exts.find(path.extension()) == m_exts.end())
                return false;
            iconType = m_exts[path.extension()];
        }
        
        bool isClicked = false;
        std::string fileNameNoExt = path.filename();
        fileNameNoExt = fileNameNoExt.substr(0, fileNameNoExt.find_last_of('.'));
        if (m_showFolder) {
            if (ImGui::TreeNodeEx(fileNameNoExt.c_str(), node_flags, reinterpret_cast<ImTextureID>(IconManager::Instance().GetTexture(iconType)))) {
                if (ImGui::IsItemClicked()) {
                    m_selectedPath = path;
                    isClicked = true;
                    if (m_callBack != nullptr)
                        m_callBack(m_selectedPath);
                }

                if (isDirectory) {
                    std::filesystem::directory_iterator end_iter;
                    for (std::filesystem::directory_iterator iter(path); iter != end_iter; ++iter) {
                        isClicked |= showPath(*iter);
                    }
                }
                ImGui::TreePop();
            }
        }else {
            if (isDirectory) {
                std::filesystem::directory_iterator end_iter;
                for (std::filesystem::directory_iterator iter(path); iter != end_iter; ++iter) {
                    isClicked |= showPath(*iter);
                }
            }else {
                if (ImGui::TreeNodeEx(fileNameNoExt.c_str(), node_flags, reinterpret_cast<ImTextureID>(IconManager::Instance().GetTexture(iconType)))) {
                    if (ImGui::IsItemClicked()) {
                        m_selectedPath = path;
                        isClicked = true;
                        if (m_callBack != nullptr)
                            m_callBack(m_selectedPath);
                    }
                    ImGui::TreePop();
                }
            }
        }
        return isClicked;
    }
}
