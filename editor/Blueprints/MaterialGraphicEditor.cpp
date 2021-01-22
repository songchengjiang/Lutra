//
//  MaterialGraphicEditor.cpp
//  Editor
//
//  Created by JasonCheng on 2021/1/18.
//

#include "MaterialGraphicEditor.h"
#include "TextureManager.h"
#include "MeshBuilder.h"
#include "ResourceBrowserWidget.h"
#include "MaterialNodeFactory.h"
#include "MaterialNode.h"
#include "BlueprintVM.h"

static const uint32_t REF_MATERIAL_SO   = 0x01000000;

namespace LutraEditor {

    MaterialGraphicEditor::MaterialGraphicEditor(const std::shared_ptr<Lutra::Scene>& scene, const std::shared_ptr<Lutra::Material>& material)
    : m_scene(scene)
    , m_material(material)
    , m_blueprint(std::shared_ptr<BlueprintVM>(new MaterialGLVM(m_material)))
    {
        m_blueprint.AddDefaultBlueprint(std::shared_ptr<BlueprintFactoryImp<UnlitMaterialNode>>(new BlueprintFactoryImp<UnlitMaterialNode>()));
        
        m_blueprint.AddTemplate(std::shared_ptr<BlueprintFactoryImp<ConstructVectorNode>>(new BlueprintFactoryImp<ConstructVectorNode>()));
        m_blueprint.AddTemplate(std::shared_ptr<BlueprintFactoryImp<SplitVectorNode>>(new BlueprintFactoryImp<SplitVectorNode>()));
        
        m_blueprint.AddTemplate(std::shared_ptr<BlueprintFactoryImp<BoolParameterNode>>(new BlueprintFactoryImp<BoolParameterNode>()));
        m_blueprint.AddTemplate(std::shared_ptr<BlueprintFactoryImp<IntParameterNode>>(new BlueprintFactoryImp<IntParameterNode>()));
        m_blueprint.AddTemplate(std::shared_ptr<BlueprintFactoryImp<FloatParameterNode>>(new BlueprintFactoryImp<FloatParameterNode>()));
        m_blueprint.AddTemplate(std::shared_ptr<BlueprintFactoryImp<Float2ParameterNode>>(new BlueprintFactoryImp<Float2ParameterNode>()));
        m_blueprint.AddTemplate(std::shared_ptr<BlueprintFactoryImp<Float3ParameterNode>>(new BlueprintFactoryImp<Float3ParameterNode>()));
        m_blueprint.AddTemplate(std::shared_ptr<BlueprintFactoryImp<Float4ParameterNode>>(new BlueprintFactoryImp<Float4ParameterNode>()));
        m_blueprint.AddTemplate(std::shared_ptr<BlueprintFactoryImp<TextureParameterNode>>(new BlueprintFactoryImp<TextureParameterNode>()));
        
        m_blueprint.AddTemplate(std::shared_ptr<BlueprintFactoryImp<SurfacePositionNode>>(new BlueprintFactoryImp<SurfacePositionNode>()));
        m_blueprint.AddTemplate(std::shared_ptr<BlueprintFactoryImp<SurfaceNormalNode>>(new BlueprintFactoryImp<SurfaceNormalNode>()));
        m_blueprint.AddTemplate(std::shared_ptr<BlueprintFactoryImp<SurfaceTangentNode>>(new BlueprintFactoryImp<SurfaceTangentNode>()));
        m_blueprint.AddTemplate(std::shared_ptr<BlueprintFactoryImp<SurfaceColorNode>>(new BlueprintFactoryImp<SurfaceColorNode>()));
        m_blueprint.AddTemplate(std::shared_ptr<BlueprintFactoryImp<SurfaceTexcoord0Node>>(new BlueprintFactoryImp<SurfaceTexcoord0Node>()));
        m_blueprint.AddTemplate(std::shared_ptr<BlueprintFactoryImp<SurfaceTexcoord1Node>>(new BlueprintFactoryImp<SurfaceTexcoord1Node>()));
        
        m_blueprint.AddTemplate(std::shared_ptr<BlueprintFactoryImp<AbsNode>>(new BlueprintFactoryImp<AbsNode>()));
        m_blueprint.AddTemplate(std::shared_ptr<BlueprintFactoryImp<AddNode>>(new BlueprintFactoryImp<AddNode>()));
        m_blueprint.AddTemplate(std::shared_ptr<BlueprintFactoryImp<CeilNode>>(new BlueprintFactoryImp<CeilNode>()));
        m_blueprint.AddTemplate(std::shared_ptr<BlueprintFactoryImp<ClampNode>>(new BlueprintFactoryImp<ClampNode>()));
        m_blueprint.AddTemplate(std::shared_ptr<BlueprintFactoryImp<DivideNode>>(new BlueprintFactoryImp<DivideNode>()));
        m_blueprint.AddTemplate(std::shared_ptr<BlueprintFactoryImp<DDXNode>>(new BlueprintFactoryImp<DDXNode>()));
        m_blueprint.AddTemplate(std::shared_ptr<BlueprintFactoryImp<DDYNode>>(new BlueprintFactoryImp<DDYNode>()));
        m_blueprint.AddTemplate(std::shared_ptr<BlueprintFactoryImp<DiscardNode>>(new BlueprintFactoryImp<DiscardNode>()));
        m_blueprint.AddTemplate(std::shared_ptr<BlueprintFactoryImp<FloorNode>>(new BlueprintFactoryImp<FloorNode>()));
        m_blueprint.AddTemplate(std::shared_ptr<BlueprintFactoryImp<FractNode>>(new BlueprintFactoryImp<FractNode>()));
        m_blueprint.AddTemplate(std::shared_ptr<BlueprintFactoryImp<ReciprocalNode>>(new BlueprintFactoryImp<ReciprocalNode>()));
        m_blueprint.AddTemplate(std::shared_ptr<BlueprintFactoryImp<MaxNode>>(new BlueprintFactoryImp<MaxNode>()));
        m_blueprint.AddTemplate(std::shared_ptr<BlueprintFactoryImp<MinNode>>(new BlueprintFactoryImp<MinNode>()));
        m_blueprint.AddTemplate(std::shared_ptr<BlueprintFactoryImp<MixNode>>(new BlueprintFactoryImp<MixNode>()));
        m_blueprint.AddTemplate(std::shared_ptr<BlueprintFactoryImp<ModNode>>(new BlueprintFactoryImp<ModNode>()));
        m_blueprint.AddTemplate(std::shared_ptr<BlueprintFactoryImp<MultiplyNode>>(new BlueprintFactoryImp<MultiplyNode>()));
        m_blueprint.AddTemplate(std::shared_ptr<BlueprintFactoryImp<NegateNode>>(new BlueprintFactoryImp<NegateNode>()));
        m_blueprint.AddTemplate(std::shared_ptr<BlueprintFactoryImp<OneMinusNode>>(new BlueprintFactoryImp<OneMinusNode>()));
        m_blueprint.AddTemplate(std::shared_ptr<BlueprintFactoryImp<RoundNode>>(new BlueprintFactoryImp<RoundNode>()));
        m_blueprint.AddTemplate(std::shared_ptr<BlueprintFactoryImp<ScaleAndOffsetNode>>(new BlueprintFactoryImp<ScaleAndOffsetNode>()));
        m_blueprint.AddTemplate(std::shared_ptr<BlueprintFactoryImp<SignNode>>(new BlueprintFactoryImp<SignNode>()));
        m_blueprint.AddTemplate(std::shared_ptr<BlueprintFactoryImp<SmoothStepNode>>(new BlueprintFactoryImp<SmoothStepNode>()));
        m_blueprint.AddTemplate(std::shared_ptr<BlueprintFactoryImp<StepNode>>(new BlueprintFactoryImp<StepNode>()));
        m_blueprint.AddTemplate(std::shared_ptr<BlueprintFactoryImp<SubtractNode>>(new BlueprintFactoryImp<SubtractNode>()));
        
        m_blueprint.AddTemplate(std::shared_ptr<BlueprintFactoryImp<ScaleCoordsNode>>(new BlueprintFactoryImp<ScaleCoordsNode>()));
        
        m_blueprint.AddTemplate(std::shared_ptr<BlueprintFactoryImp<DeltaTimeNode>>(new BlueprintFactoryImp<DeltaTimeNode>()));
        m_blueprint.AddTemplate(std::shared_ptr<BlueprintFactoryImp<ElapsedTimeNode>>(new BlueprintFactoryImp<ElapsedTimeNode>()));
        
        
        m_meshSO = m_scene->CreateSceneObject("MaterialModel");
        m_meshSO.AddComponent<Lutra::MeshFilter>().MeshPtr = SphereMeshBuilder::Build(1.0f, 36, 18);
        m_meshSO.AddComponent<Lutra::MeshRenderer>().Materials.push_back(m_material);
        m_meshSO.GetComponent<Lutra::Tag>().VisibleTag = REF_MATERIAL_SO;
        
        m_cameraSO = m_scene->CreateSceneObject("MaterialCamera");
        auto& camera = m_cameraSO.AddComponent<Lutra::Camera>();
        camera.VisibleMask = REF_MATERIAL_SO;
        m_colorTexture = Lutra::ResourceManager::Instance().CreateResource<Lutra::RenderTexture>(512, 512, Lutra::TextureFormat::RGBA8, Lutra::TextureFormat::D24S8);
        camera.RenderTexture_ = m_colorTexture;
        camera.RenderTexture_->SetClearColor(glm::vec4(0.1f, 0.1f, 0.1f, 1.0f));
        
        m_cameraManipulator.reset(new CameraManipulator(m_cameraSO, glm::vec3(0.0f, 0.0f, 5.0f), glm::vec3(0.0f, 0.0f, 0.0f)));
    }

    MaterialGraphicEditor::~MaterialGraphicEditor()
    {
        m_scene->DestroySceneObject(m_meshSO);
        m_scene->DestroySceneObject(m_cameraSO);
    }

    void MaterialGraphicEditor::Open()
    {
        m_isOpen = true;
    }

    void MaterialGraphicEditor::Close()
    {
        m_isOpen = false;
    }

    void MaterialGraphicEditor::OnGUI(uint32_t width, uint32_t height)
    {
        if (!m_isOpen)
            return;
        ImGuiWindowFlags window_flags = ImGuiWindowFlags_None;
        ImGui::SetNextWindowSize(ImVec2(800, 600), ImGuiCond_FirstUseEver);
        ImGui::Begin("Material", &m_isOpen, window_flags);
        
        auto size = ImGui::GetContentRegionAvail();
        
        ImGui::BeginGroup();
        ImGui::BeginChild("View", ImVec2(300, 300));
        m_cameraManipulator->Update(width, height);
        ImGui::Image(reinterpret_cast<ImTextureID>(m_colorTexture->GetTextureID()), {ImGui::GetContentRegionAvailWidth(), ImGui::GetContentRegionAvailWidth()}, {0, 1}, {1, 0});
        ImGui::EndChild();
        
        ImGui::BeginChild("Property", ImVec2(300, size.y - 310));
        bool isChanged = false;
        for (auto& node : m_blueprint.GetSelectedNodes()) {
            isChanged |= node->OnGUI();
        }
        if (isChanged) {
            m_blueprint.Compile();
        }
        ImGui::EndChild();
        ImGui::EndGroup();
        
        ImGui::SameLine();
        ImGui::BeginGroup();
        ImGui::BeginChild("Blueprint", ImVec2(ImGui::GetContentRegionAvailWidth(), size.y), false, ImGuiWindowFlags_None);
        if (m_blueprint.OnGui()) {
            m_blueprint.Compile();
        }
        ImGui::EndChild();
        ImGui::EndGroup();
        
        ImGui::End();
    }

}
