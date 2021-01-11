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
#include "Windows/FileBrowserWindow.h"
#include "Windows/PropertyWindow.h"
#include "Windows/ProjectWindow.h"
#include "Windows/SceneWindow.h"
#include "Windows/LogWindow.h"

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

        glfwDestroyWindow(m_window);

        glfwTerminate();
    }

    void Application::onInit()
    {
        onGUIInit();
        
        auto scene = std::shared_ptr<Lutra::Scene>(new Lutra::Scene());
        m_engine.AddScene(scene);
        scene->AppendSystem<Lutra::TransformSystem>();
        scene->AppendSystem<Lutra::SpriteSystem>();
        scene->AppendSystem<Lutra::MeshFilterSystem>();
        
        IconManager::Instance().LoadTextue(IconType::Folder, "Resources/Icons/folder.png");
        IconManager::Instance().LoadTextue(IconType::Texture, "Resources/Icons/image.png");
        IconManager::Instance().LoadTextue(IconType::Camera, "Resources/Icons/camera.png");
        IconManager::Instance().LoadTextue(IconType::SceneObject, "Resources/Icons/object.png");
        IconManager::Instance().LoadTextue(IconType::Tag, "Resources/Icons/tag.png");
        IconManager::Instance().LoadTextue(IconType::Square, "Resources/Icons/square.png");
        
        auto propertyWindow = new PropertyWindow();
        propertyWindow->Open();
        m_guiWindows.emplace_back(propertyWindow);
        
        auto fileBrowserWindow = new FileBrowserWindow("/Volumes/Samsung_T5/Dev/Lutra", propertyWindow);
        fileBrowserWindow->Open();
        m_guiWindows.emplace_back(fileBrowserWindow);
        
        auto projectWindow = new ProjectWindow(scene, propertyWindow);
        projectWindow->Open();
        m_guiWindows.emplace_back(projectWindow);
    
        auto sceneWindow = new SceneWindow(scene);
        sceneWindow->Open();
        m_guiWindows.emplace_back(sceneWindow);
        
        auto logWindow = new LogWindow(scene);
        logWindow->Open();
        m_guiWindows.emplace_back(logWindow);
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

}
