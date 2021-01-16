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
in vec3 a_position;
//in vec3 a_normal;
in vec2 a_texcoord0;

uniform mat4 u_ModelViewProjMat;
uniform mat3 u_NormalWorldMat;

//out vec3 v_normal;
out vec2 v_texcoord0;

void main() {
    v_texcoord0 = a_texcoord0;
    //v_normal = u_NormalWorldMat * a_normal;
    gl_Position = u_ModelViewProjMat * vec4(a_position, 1.0);
}
)";

const char* Fragment_Shader = R"(
uniform sampler2D u_BaseTex;

//in vec3 v_normal;
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
    
    auto texture = Lutra::ResourceManager::Instance().CreateResource<Lutra::Texture2D>(width, height, channels == 3? Lutra::TextureFormat::RGB8: Lutra::TextureFormat::RGBA8);
    texture->SetData(data, width * height * channels);
    
    stbi_image_free(data);
    
    return texture;
}

void BuildResources(const std::string& path)
{
    Lutra::ResourceManifest::Instance().SetRootDirectoty(path);
    
    auto tex = LoadTexture(path + "Grid.png");
    auto tex1 = LoadTexture(path + "Grid_2.jpg");
    Lutra::ResourceManager::Instance().SaveResource(path + "Grid.tex", tex);
    Lutra::ResourceManager::Instance().SaveResource(path + "Grid_2.tex", tex1);
    
    auto material = Lutra::ResourceManager::Instance().CreateResource<Lutra::Material>();
    material->AddPass(std::shared_ptr<Lutra::Pass>(new Lutra::Pass({
        Vertex_Shader,
        Fragment_Shader,
        Lutra::BlendMode::Disabled,
        Lutra::CullMode::Back,
        true,
        true,
        true,
        glm::bvec4(true)
    })));
    material->GetPass(0)->GetShader().SetSampler("u_BaseTex", tex);
    Lutra::ResourceManager::Instance().SaveResource(path + "material.mat", material);
    
    auto material_2 = Lutra::ResourceManager::Instance().CreateResource<Lutra::Material>();
    material_2->AddPass(std::shared_ptr<Lutra::Pass>(new Lutra::Pass({
        Vertex_Shader,
        Fragment_Shader,
        Lutra::BlendMode::Disabled,
        Lutra::CullMode::Back,
        true,
        true,
        true,
        glm::bvec4(true)
    })));
    material_2->GetPass(0)->GetShader().SetSampler("u_BaseTex", tex1);
    Lutra::ResourceManager::Instance().SaveResource(path + "material_2.mat", material_2);
    
    std::shared_ptr<Lutra::Mesh> mesh = Lutra::ResourceManager::Instance().CreateResource<Lutra::Mesh>();
    mesh->Vertices.push_back(glm::vec3(0.0f, 0.0f, 0.0f));
    mesh->Vertices.push_back(glm::vec3(1.0f, 0.0f, 0.0f));
    mesh->Vertices.push_back(glm::vec3(0.0f, 1.0f, 0.0f));
    mesh->Vertices.push_back(glm::vec3(1.0f, 1.0f, 0.0f));
    
    mesh->Normals.push_back(glm::vec3(0.0f, 0.0f, 1.0f));
    mesh->Normals.push_back(glm::vec3(0.0f, 0.0f, 1.0f));
    mesh->Normals.push_back(glm::vec3(0.0f, 0.0f, 1.0f));
    mesh->Normals.push_back(glm::vec3(0.0f, 0.0f, 1.0f));
    
    mesh->Texcoord0.push_back(glm::vec2(0.0f, 0.0f));
    mesh->Texcoord0.push_back(glm::vec2(1.0f, 0.0f));
    mesh->Texcoord0.push_back(glm::vec2(0.0f, 1.0f));
    mesh->Texcoord0.push_back(glm::vec2(1.0f, 1.0f));
    
    mesh->SubMeshList.emplace_back(Lutra::SubMesh{Lutra::PrimitiveType::Triangle, 0, {0, 1, 2}});
    mesh->SubMeshList.emplace_back(Lutra::SubMesh{Lutra::PrimitiveType::Triangle, 1, {1, 3, 2}});
    
    Lutra::ResourceManager::Instance().SaveResource(path + "quat.mesh", mesh);
    
    Lutra::ResourceManifest::Instance().Save(path + "Manifest.yaml");
    Lutra::ResourceManifest::Instance().Load(path + "Manifest.yaml");
}

std::shared_ptr<Lutra::RenderTexture> MainRenderTexture;

void Init(Lutra::Engine& engine, int width, int height)
{
    std::string path = "/Users/JasonCheng/Desktop/";
    BuildResources(path);
    
    auto scene = std::shared_ptr<Lutra::Scene>(new Lutra::Scene());
    scene->AppendSystem<Lutra::TransformSystem>();
    scene->AppendSystem<Lutra::SpriteSystem>();
    scene->AppendSystem<Lutra::MeshFilterSystem>();
    
    auto gridTexture = Lutra::ResourceManager::Instance().LoadResource<Lutra::Texture>(path + "Grid.tex");
    auto gridTexture_2 = Lutra::ResourceManager::Instance().LoadResource<Lutra::Texture>(path + "Grid_2.tex");
    auto material = Lutra::ResourceManager::Instance().LoadResource<Lutra::Material>(path + "material.mat");
    auto material_2 = Lutra::ResourceManager::Instance().LoadResource<Lutra::Material>(path + "material_2.mat");
    auto mesh = Lutra::ResourceManager::Instance().LoadResource<Lutra::Mesh>(path + "quat.mesh");
    
    for (int i = 0; i < 2; ++i) {
        auto so = scene->CreateSceneObject("Sprite");
        auto& sr = so.AddComponent<Lutra::SpriteRenderer>();
        sr.Size = {1.0f, 1.0f};
        sr.Anchor = {0.5f, 0.5f};
        
        sr.Materials.push_back(material);
        sr.Materials.push_back(material_2);
        
        auto& transform = so.GetComponent<Lutra::Transform>();
        transform.Position = glm::vec3(-1.0f + 2.0f * i, 0.0f, 0.0f);
        transform.Rotation = glm::vec3(0.0f, 0.0f, 0.0f);
        
        auto& mf = so.AddComponent<Lutra::MeshFilter>();
        mf.MeshPtr = mesh;
    }
    
//    Lutra::SceneObject rttCameraSO = scene->CreateSceneObject("RTTCamera");
//    auto& rttCamera = rttCameraSO.AddComponent<Lutra::Camera>();
//    rttCamera.AspectRadio = (float)width / height;
//
//    auto renderTexture = new Lutra::RenderTexture(width, height);
//    renderTexture->ColorAttachments.push_back(rttTexture);
//    rttCamera.RenderTarget_ = std::shared_ptr<Lutra::RenderTarget>(renderTexture);
//    rttCamera.RenderTarget_->SetClearColor(glm::vec4(0.8f));
    
    MainRenderTexture= Lutra::ResourceManager::Instance().CreateResource<Lutra::RenderTexture>(1024, 1024, Lutra::TextureFormat::RGBA8, Lutra::TextureFormat::D24S8);
    auto cameraLeftSO = scene->CreateSceneObject("LeftCamera");
    {
        auto& camera = cameraLeftSO.AddComponent<Lutra::Camera>();
        camera.Fov = 60.0f;
        camera.AspectRadio = (float)width * 0.5f / height;
        camera.Viewport_ = {0.0f, 0.0f, 0.5f, 1.0f};
        camera.RenderTexture_ = MainRenderTexture;
        auto& cameraTrans = cameraLeftSO.GetComponent<Lutra::Transform>();
        cameraTrans.Position = glm::vec3(0.0f, 0.0f, 5.0f);
    }
    
    auto cameraRightSO = scene->CreateSceneObject("RightCamera");
    {
        auto& camera = cameraRightSO.AddComponent<Lutra::Camera>();
        camera.Fov = 60.0f;
        camera.AspectRadio = (float)width * 0.5f / height;
        camera.Viewport_ = {0.5f, 0.0f, 1.0f, 1.0f};
        camera.RenderTexture_ = MainRenderTexture;
        
        auto& cameraTrans = cameraRightSO.GetComponent<Lutra::Transform>();
        cameraTrans.Position = glm::vec3(0.0f, 0.0f, 2.0f);
    }
    
    scene->GetEventDispatcher().Connect<Lutra::WindowResizeEvent>([cameraLeftSO, cameraRightSO](Lutra::Event& event) -> bool{
        Lutra::WindowResizeEvent& winEvent = static_cast<Lutra::WindowResizeEvent&>(event);
        cameraLeftSO.GetComponent<Lutra::Camera>().AspectRadio = (float)winEvent.GetWidth() * 0.5f / winEvent.GetHeight();
        cameraRightSO.GetComponent<Lutra::Camera>().AspectRadio = (float)winEvent.GetWidth() * 0.5f / winEvent.GetHeight();
        return true;
    });
    
    //Lutra::SceneLoader sl;
    //sl.Save("/Users/JasonCheng/Desktop/scene.scene", scene);
    //scene = sl.Load("/Users/JasonCheng/Desktop/scene.scene");
    engine.AddScene(scene);
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

void EngineUpdate(Lutra::Engine& engine, double frameTime, int width, int height)
{
    engine.GetScene(0)->GetEventDispatcher().Send(std::shared_ptr<Lutra::WindowResizeEvent>(new Lutra::WindowResizeEvent(width, height)));
    engine.OnUpdateFrame(frameTime);
}

void ImGUIUpdate()
{
    // Start the Dear ImGui frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
    
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    ImGui::Image((ImTextureID)MainRenderTexture->GetTextureID(), {800, 600}, {0, 1}, {1, 0});
    
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
        glfwGetFramebufferSize(window, &width, &height);
        double currentTime = glfwGetTime();
        EngineUpdate(engine, currentTime - previousTime, width, height);
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
