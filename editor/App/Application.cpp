//
//  Application.cpp
//  Editor
//
//  Created by JasonCheng on 2021/1/8.
//

#include "Application.h"
#include <glad/glad.h>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <imguiFileDialog/ImGuiFileDialog.h>
#include <imguiFileDialog/res/CustomFont.h>
#include <imguiFileDialog/res/CustomFont.cpp>
#include "Windows/ResourceWindow.h"
#include "Windows/PropertyWindow.h"
#include "Windows/ProjectWindow.h"
#include "Windows/SceneWindow.h"
#include "Windows/GameWindow.h"
#include "Windows/LogWindow.h"
#include "Builders/SceneBuilder.h"


namespace LutraEditor {

    Application::Application()
    {
        glfwInit();
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        
        m_window = glfwCreateWindow(1280, 720, "Editor", NULL, NULL);
        if (!m_window) {
            glfwTerminate();
        }
        
    }

    Application::~Application()
    {
        
    }

    void Application::Run()
    {
        glfwMakeContextCurrent(m_window);
        
        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
            return;
        }
        onInit();
        
        double previousTime = glfwGetTime();
        while (!glfwWindowShouldClose(m_window))
        {
            double currentTime = glfwGetTime();
            onUpdate(currentTime - previousTime);
            previousTime = currentTime;
            glfwSwapBuffers(m_window);
            glfwPollEvents();
        }
        
        saveScene();

        glfwDestroyWindow(m_window);

        glfwTerminate();
    }

    void Application::onInit()
    {
        onGUIInit();
        
        IconManager::Instance().LoadTextue(IconType::Folder, "Resources/Icons/folder.png");
        IconManager::Instance().LoadTextue(IconType::Texture, "Resources/Icons/image.png");
        IconManager::Instance().LoadTextue(IconType::Material, "Resources/Icons/material.png");
        IconManager::Instance().LoadTextue(IconType::Mesh, "Resources/Icons/mesh.png");
        IconManager::Instance().LoadTextue(IconType::Camera, "Resources/Icons/camera.png");
        IconManager::Instance().LoadTextue(IconType::SceneObject, "Resources/Icons/object.png");
        IconManager::Instance().LoadTextue(IconType::Tag, "Resources/Icons/tag.png");
        IconManager::Instance().LoadTextue(IconType::Square, "Resources/Icons/square.png");
    }

    void Application::onUpdate(double frameTime)
    {
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        m_engine.OnUpdateFrame(frameTime);
        onGUIUpdate();
    }

    void Application::onGUIInit()
    {
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO(); (void)io;
        //io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
        //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
        io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
        
        io.Fonts->AddFontDefault();
        static const ImWchar icons_ranges[] = { ICON_MIN_IGFD, ICON_MAX_IGFD, 0 };
        ImFontConfig icons_config; icons_config.MergeMode = true; icons_config.PixelSnapH = true;
        io.Fonts->AddFontFromMemoryCompressedBase85TTF(FONT_ICON_BUFFER_NAME_IGFD, 15.0f, &icons_config, icons_ranges);
        
        // Setup Dear ImGui style
        ImGui::StyleColorsDark();
        ImGuiStyle& style = ImGui::GetStyle();
        style.FrameRounding = 8;

        // Setup Platform/Renderer backends
        ImGui_ImplGlfw_InitForOpenGL(m_window, true);
        ImGui_ImplOpenGL3_Init(nullptr);
    }

    void Application::onGUIUpdate()
    {
        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        
        ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
        ImGuiViewport* viewport = ImGui::GetMainViewport();
        ImGui::SetNextWindowPos(viewport->GetWorkPos());
        ImGui::SetNextWindowSize(viewport->GetWorkSize());
        ImGui::SetNextWindowViewport(viewport->ID);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
        window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
        window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

        bool isOpen = true;
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
        ImGui::Begin("DockSpace", &isOpen, window_flags);
        ImGui::PopStyleVar();
        ImGui::PopStyleVar(2);
        
        onMenu();

        // DockSpace
        ImGuiID dockspace_id = ImGui::GetID("DockSpace");
        ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), ImGuiDockNodeFlags_None);

        int width, height;
        glfwGetWindowSize(m_window, &width, &height);
        for (auto& win : m_guiWindows) {
            win->OnGUI(width, height);
        }
        
        ImGui::End();
        
        static bool show_demo_window = true;
        if (show_demo_window)
            ImGui::ShowDemoWindow(&show_demo_window);
        
        
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    }

    void Application::onMenu()
    {
        bool isNewScene = false;
        if (ImGui::BeginMenuBar())
        {
            if (ImGui::BeginMenu("File"))
            {
                if (ImGui::MenuItem("New Scene...")) {
                    isNewScene = true;
                }
                if (ImGui::MenuItem("Open Scene...")) {
                    igfd::ImGuiFileDialog::Instance()->OpenDialog("OpenSceneDlgKey", ICON_IGFD_FOLDER_OPEN "Choose Scene", 0, ".");
                }
                
                if (m_scene != nullptr) {
                    if (ImGui::MenuItem("Save Scene")) {
                        saveScene();
                    }
                }
                    
                ImGui::EndMenu();
            }
            if (ImGui::BeginMenu("Edit"))
            {
                if (ImGui::MenuItem("Undo", "Ctrl + Z")) {
                    
                }
                if (ImGui::MenuItem("Redo", "Ctrl + Shift + Z")) {
                    
                }
                ImGui::Separator();
                if (ImGui::MenuItem("Cut", "Ctrl + X")) {
                    
                }
                if (ImGui::MenuItem("Copy", "Ctrl + C")) {
                    
                }
                if (ImGui::MenuItem("Paste", "Ctrl + V")) {
                    
                }
                ImGui::EndMenu();
            }
            ImGui::EndMenuBar();
        }
        
        if (isNewScene) {
            ImGui::OpenPopup("NewScenePopup");
        }
        
        static std::string sceneFolder;
        ImGui::SetNextWindowSize({200, 80});
        if (ImGui::BeginPopupModal("NewScenePopup", NULL, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar)) {
            static char name[128] = "Default";
            ImGui::InputText("Name", name, IM_ARRAYSIZE(name));
            if (ImGui::Button("OK", ImVec2(50, 0))) {
                sceneFolder = name;
                igfd::ImGuiFileDialog::Instance()->OpenDialog("NewSceneDlgKey", ICON_IGFD_FOLDER_OPEN "Choose Folder", 0, ".");
                ImGui::CloseCurrentPopup();
            }
            ImGui::SetItemDefaultFocus();
            ImGui::SameLine();
            if (ImGui::Button("Cancel", ImVec2(50, 0))) {
                ImGui::CloseCurrentPopup();
            }
            
            ImGui::EndPopup();
        }
        
        ImGui::SetNextWindowSize({800, 600});
        if (igfd::ImGuiFileDialog::Instance()->FileDialog("NewSceneDlgKey", ImGuiWindowFlags_NoCollapse, {600, 200}, {1200, 800}))
        {
            if (igfd::ImGuiFileDialog::Instance()->IsOk == true)
            {
                std::string folder = igfd::ImGuiFileDialog::Instance()->GetCurrentPath() + "/" + sceneFolder;
                std::filesystem::create_directory(folder);
                newScene(folder);
            }
            // close
            igfd::ImGuiFileDialog::Instance()->CloseDialog();
        }
        
        if (igfd::ImGuiFileDialog::Instance()->FileDialog("OpenSceneDlgKey", ImGuiWindowFlags_NoCollapse, {600, 200}, {1200, 800}))
        {
            if (igfd::ImGuiFileDialog::Instance()->IsOk == true)
            {
                std::string folder = igfd::ImGuiFileDialog::Instance()->GetCurrentPath();
                loadScene(folder);
            }
            // close
            igfd::ImGuiFileDialog::Instance()->CloseDialog();
        }
    }

    void Application::newScene(const std::string& path)
    {
        if (std::filesystem::exists(path + "/Scene.scene"))
            return;
        Lutra::ResourceManifest::Instance().Load("");
        m_engine.RemoveScene(m_scene);
        m_scene = SceneBuilder::BuildSimpleScene(path);
        m_engine.AddScene(m_scene);
        initScene(path);
    }

    void Application::loadScene(const std::string& path)
    {
        if (!std::filesystem::exists(path + "/Scene.scene"))
            return;
        Lutra::ResourceManifest::Instance().Load(path + "/manifest.yaml");
        m_engine.RemoveScene(m_scene);
        Lutra::SceneLoader loader;
        m_scene = loader.Load(path + "/Scene.scene");
        m_engine.AddScene(m_scene);
        initScene(path);
    }

    void Application::initScene(const std::string& path)
    {
        m_guiWindows.clear();
        
        auto propertyWindow = new PropertyWindow();
        propertyWindow->Open();
        m_guiWindows.emplace_back(propertyWindow);
        
        auto resourceWindow = new ResourceWindow(path, propertyWindow);
        resourceWindow->Open();
        m_guiWindows.emplace_back(resourceWindow);
        
        auto projectWindow = new ProjectWindow(m_scene, propertyWindow);
        projectWindow->Open();
        m_guiWindows.emplace_back(projectWindow);
    
        auto sceneWindow = new SceneWindow(m_scene);
        sceneWindow->Open();
        m_guiWindows.emplace_back(sceneWindow);
        
        auto gameWindow = new GameWindow(m_scene);
        gameWindow->Open();
        m_guiWindows.emplace_back(gameWindow);
        
        auto logWindow = new LogWindow(m_scene);
        logWindow->Open();
        m_guiWindows.emplace_back(logWindow);
        
        m_projectFolder = path;
    }

    void Application::saveScene()
    {
        Lutra::SceneLoader loader;
        loader.Save(m_projectFolder + "/Scene.scene", m_scene);
        Lutra::ResourceManifest::Instance().Save(m_projectFolder + "/manifest.yaml");
    }

}
