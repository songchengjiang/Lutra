//
//  main.cpp
//  Examples
//
//  Created by JasonCheng on 2021/1/4.
//

#include <iostream>
#include "Lutra.h"
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include "stb_image.h"
#include <glad/glad.h>
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

const char* Vertex_Shader = R"(
#version 410
layout(location = 0) in vec3 a_position;
layout(location = 1) in vec3 a_normal;
layout(location = 2) in vec2 a_texcoord0;

uniform mat4 u_ModelViewProjMat;
uniform mat3 u_NormalWorldMat;

out vec3 v_normal;
out vec2 v_texcoord0;

void main() {
    v_texcoord0 = a_texcoord0;
    v_normal = u_NormalWorldMat * a_normal;
    gl_Position = u_ModelViewProjMat * vec4(a_position, 1.0);
}
)";

const char* Fragment_Shader = R"(
#version 410
uniform sampler2D u_BaseTex;

in vec3 v_normal;
in vec2 v_texcoord0;

out vec4 FragColor;
void main() {
    vec4 color = texture(u_BaseTex, v_texcoord0);
    FragColor = color;
}
)";

std::shared_ptr<Lutra::Texture> LoadTexture(const std::string& path)
{
    int width, height, channels;
    stbi_set_flip_vertically_on_load(1);
    stbi_uc* data = nullptr;
    {
        data = stbi_load(path.c_str(), &width, &height, &channels, 0);
    }
    
    Lutra::Texture2D* texture = new Lutra::Texture2D(width, height, channels == 3? Lutra::TextureFormat::RGB8: Lutra::TextureFormat::RGBA8);
    texture->SetData(data, width * height * channels);
    
    stbi_image_free(data);
    
    return std::shared_ptr<Lutra::Texture>(texture);
}

void Init(Lutra::Engine& engine, int width, int height)
{
    auto scene = std::shared_ptr<Lutra::Scene>(new Lutra::Scene());
    engine.AddScene(scene);
    scene->AppendSystem<Lutra::TransformSystem>();
    scene->AppendSystem<Lutra::SpriteSystem>();
    scene->AppendSystem<Lutra::MeshFilterSystem>();
    
    auto so = scene->CreateSceneObject("Sprite");
    auto& sr = so.AddComponent<Lutra::SpriteRenderer>();
    sr.Size = {1.0f, 1.0f};
    sr.Anchor = {0.5f, 0.5f};
    
    std::shared_ptr<Lutra::Material> material{new Lutra::Material};
    material->AddPass(Lutra::Pass({
        Vertex_Shader,
        Fragment_Shader,
        Lutra::BlendMode::Disabled,
        Lutra::CullMode::FrontAndBack,
        true,
        true,
        true,
        glm::bvec4(true)
    }));
    sr.Materials.push_back(material);
    
    auto rttTexture = std::shared_ptr<Lutra::Texture>(new Lutra::Texture2D(width, height, Lutra::TextureFormat::RGBA8));
    auto texture = LoadTexture("/Users/JasonCheng/Desktop/Grid.png");
    material->GetPass(0).GetShader().SetSampler("u_BaseTex", rttTexture);
    
    auto& transform = so.GetComponent<Lutra::Transform>();
    transform.Position = glm::vec3(0.0f, 0.0f, 0.0f);
    transform.Rotation = glm::vec3(0.0f, 0.0f, 0.0f);
    
    auto& mf = so.AddComponent<Lutra::MeshFilter>();
    mf.MeshPtr = std::shared_ptr<Lutra::Mesh>(new Lutra::Mesh);
    mf.MeshPtr->Vertices.push_back(glm::vec3(0.0f, 0.0f, 0.0f));
    mf.MeshPtr->Vertices.push_back(glm::vec3(1.0f, 0.0f, 0.0f));
    mf.MeshPtr->Vertices.push_back(glm::vec3(0.0f, 1.0f, 0.0f));
    mf.MeshPtr->Vertices.push_back(glm::vec3(1.0f, 1.0f, 0.0f));
    
    mf.MeshPtr->Normals.push_back(glm::vec3(0.0f, 0.0f, 1.0f));
    mf.MeshPtr->Normals.push_back(glm::vec3(0.0f, 0.0f, 1.0f));
    mf.MeshPtr->Normals.push_back(glm::vec3(0.0f, 0.0f, 1.0f));
    mf.MeshPtr->Normals.push_back(glm::vec3(0.0f, 0.0f, 1.0f));
    
    mf.MeshPtr->Texcoord0.push_back(glm::vec2(0.0f, 0.0f));
    mf.MeshPtr->Texcoord0.push_back(glm::vec2(1.0f, 0.0f));
    mf.MeshPtr->Texcoord0.push_back(glm::vec2(0.0f, 1.0f));
    mf.MeshPtr->Texcoord0.push_back(glm::vec2(1.0f, 1.0f));
    
    mf.MeshPtr->SubMeshList.emplace_back(Lutra::SubMesh{0, {0, 1, 2, 1, 3, 2}});
    
    auto rttCameraSO = scene->CreateSceneObject("RTTCamera");
    auto& rttCamera = rttCameraSO.AddComponent<Lutra::Camera>();
    rttCamera.AspectRadio = (float)width / height;
    
    auto renderTexture = new Lutra::RenderTexture(width, height);
    renderTexture->ColorAttachments.push_back(rttTexture);
    rttCamera.RenderTarget_ = std::shared_ptr<Lutra::RenderTarget>(renderTexture);
    rttCamera.RenderTarget_->SetClearColor(glm::vec4(1.0f));
    
    auto cameraSO = scene->CreateSceneObject("Camera");
    auto& camera = cameraSO.AddComponent<Lutra::Camera>();
    camera.Fov = 60.0f;
    camera.AspectRadio = (float)width / height;
    
    camera.RenderTarget_ = std::shared_ptr<Lutra::RenderTarget>(new Lutra::RenderWindow(width, height));
    camera.RenderTarget_->SetClearColor(glm::vec4(0.5f, 0.5f, 0.5f, 1.0f));
    
    auto& cameraTrans = cameraSO.GetComponent<Lutra::Transform>();
    cameraTrans.Position = glm::vec3(0.0f, 0.0f, 2.0f);
    //cameraTrans.Rotation = glm::vec3(0.1f, 0.1f, 0.1f);
}

void InitImGUI(GLFWwindow* window)
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsClassic();

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(nullptr);
}

void EngineUpdate(Lutra::Engine& engine, double frameTime)
{
    engine.OnUpdateFrame(frameTime);
}

void ImGUIUpdate()
{
    // Start the Dear ImGui frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
    
    static bool show_demo_window = true;
    if (show_demo_window)
        ImGui::ShowDemoWindow(&show_demo_window);
    
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

int main(int argc, const char * argv[]) {
    if (!glfwInit())
        exit(EXIT_FAILURE);

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    
    GLFWwindow* window = glfwCreateWindow(800, 600, "Simple example", NULL, NULL);
    if (!window) {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }
    glfwMakeContextCurrent(window);
    
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        exit(EXIT_FAILURE);
    }
    
    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    
    Lutra::Engine engine;
    Init(engine, width, height);
    InitImGUI(window);
    
    double previousTime = glfwGetTime();
    while (!glfwWindowShouldClose(window))
    {
        double currentTime = glfwGetTime();
        EngineUpdate(engine, currentTime - previousTime);
        ImGUIUpdate();
        previousTime = currentTime;
        
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);

    glfwTerminate();
    exit(EXIT_SUCCESS);
    
    return 0;
}
