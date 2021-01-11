//
//  FileBrowserWindow.cpp
//  Editor
//
//  Created by JasonCheng on 2021/1/9.
//

#include "FileBrowserWindow.h"
#include "PropertyWindow.h"
#include "IconManager.h"

namespace LutraEditor {

    FileBrowserWindow::FileBrowserWindow(const std::string& root, PropertyWindow* propertyWindow)
    : m_root(root)
    , m_propertyWindow(propertyWindow)
    {
    }

    FileBrowserWindow::~FileBrowserWindow()
    {
        
    }

    void FileBrowserWindow::Open()
    {
        m_isOpen = true;
    }

    void FileBrowserWindow::Close()
    {
        m_isOpen = false;
    }

    void FileBrowserWindow::OnGUI(uint32_t width, uint32_t height)
    {
        ImGui::SetNextWindowSize(ImVec2(200, 300), ImGuiCond_FirstUseEver);
        ImGuiWindowFlags window_flags = ImGuiWindowFlags_None;
        ImGui::Begin("Browser", &m_isOpen, window_flags);
        showPath(m_root);
        ImGui::End();
    }

void FileBrowserWindow::showPath(const std::filesystem::path& path)
{
    if (!std::filesystem::exists(path))
        return;
    
    ImGuiTreeNodeFlags node_flags = ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_SpanFullWidth;
    if (m_selectedPath == path)
        node_flags |= ImGuiTreeNodeFlags_Selected;
    
    bool isDirectory = std::filesystem::is_directory(path);
    if (!isDirectory)
        node_flags |= ImGuiTreeNodeFlags_Leaf;
    
    std::string fileNameNoExt = path.filename();
    fileNameNoExt = fileNameNoExt.substr(0, fileNameNoExt.find_last_of('.'));
    if (ImGui::TreeNodeEx(fileNameNoExt.c_str(), node_flags,  isDirectory? reinterpret_cast<ImTextureID>(IconManager::Instance().GetTexture(IconType::Folder)): NULL)) {
        if (ImGui::IsItemClicked()) {
            m_selectedPath = path;
        }

        if (isDirectory) {
            std::filesystem::directory_iterator end_iter;
            for (std::filesystem::directory_iterator iter(path); iter != end_iter; ++iter) {
                showPath(*iter);
            }
        }
        ImGui::TreePop();
    }
}
}
