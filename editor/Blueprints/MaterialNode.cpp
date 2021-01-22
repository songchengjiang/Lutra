//
//  MaterialNode.cpp
//  Editor
//
//  Created by JasonCheng on 2021/1/19.
//

#include "MaterialNode.h"
#include "ResourceBrowserWidget.h"
#include "BlueprintVM.h"
#include "Blueprint.h"

namespace LutraEditor {

    static std::string GetInputId()
    {
        static int id = 1;
        return "Input_" + std::to_string(id++);
    }

    static std::string GetOutputId()
    {
        static int id = 1;
        return "Output_" + std::to_string(id++);
    }

    void UnlitMaterialNode::OnInit(BlueprintVM* vm)
    {
        this->Title = "Shader";
        this->Name  = "Unlit";
        this->Color = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
        this->addInput({"Color.rgba", PinType::Float4, GetInputId()});
        this->addInput({"WorldPosition.xyz", PinType::Float3, GetInputId()});
        this->addInput({"WorldNormal.xyz", PinType::Float3, GetInputId()});
        this->addInput({"WorldTangent.xyz", PinType::Float3, GetInputId()});
    }

    bool UnlitMaterialNode::Compile(BlueprintVM* vm)
    {
        auto vmImp = static_cast<MaterialVM*>(vm);
        
        auto link = m_bluePrint->FindLink(m_inputs[0].GetID());
        if (link) {
            vmImp->GetFragmentFunctions()->Assignment({link->StartPin.ScriptName, glm::vec4(0.0f)}, {"v_fragColor", glm::vec4(0.0f)});
        }else {
            vmImp->GetFragmentFunctions()->Assignment({"One", 1.0f}, {"v_fragColor", glm::vec4(0.0f)});
        }
        
        link = m_bluePrint->FindLink(m_inputs[1].GetID());
        if (link) {
            vmImp->GetVertexFunctions()->Assignment({link->StartPin.ScriptName, glm::vec3(0.0f)}, {"v_worldPosition", glm::vec3(0.0f)});
        }
        
        link = m_bluePrint->FindLink(m_inputs[2].GetID());
        if (link) {
            vmImp->GetVertexFunctions()->Assignment({link->StartPin.ScriptName, glm::vec3(0.0f)}, {"v_worldNormal", glm::vec3(0.0f)});
        }
        
        link = m_bluePrint->FindLink(m_inputs[3].GetID());
        if (link) {
            vmImp->GetVertexFunctions()->Assignment({link->StartPin.ScriptName, glm::vec3(0.0f)}, {"v_worldTangent", glm::vec3(0.0f)});
        }
        
        
        return true;
    }

    void ConstructVectorNode::OnInit(BlueprintVM* vm)
    {
        this->Title = "Construct Vector 4";
        this->Color = glm::vec4(0.5f, 0.5f, 0.5f, 1.0f);
        this->addInput({"Value.x", PinType::Float, GetInputId()});
        this->addInput({"Value.y", PinType::Float, GetInputId()});
        this->addInput({"Value.z", PinType::Float, GetInputId()});
        this->addInput({"Value.w", PinType::Float, GetInputId()});
        this->addOutput({"Value.xyzw", PinType::Float4, GetOutputId()});
    }

    bool ConstructVectorNode::Compile(BlueprintVM* vm)
    {
        auto vmImp = static_cast<MaterialVM*>(vm);
        auto link = m_bluePrint->FindLink(m_inputs[0].GetID());
        std::string x = link? link->StartPin.ScriptName: m_inputs[0].ScriptName;
        
        link = m_bluePrint->FindLink(m_inputs[1].GetID());
        std::string y = link? link->StartPin.ScriptName: m_inputs[1].ScriptName;
        
        link = m_bluePrint->FindLink(m_inputs[2].GetID());
        std::string z = link? link->StartPin.ScriptName: m_inputs[2].ScriptName;
        
        link = m_bluePrint->FindLink(m_inputs[3].GetID());
        std::string w = link? link->StartPin.ScriptName: m_inputs[3].ScriptName;
        
        vmImp->GetVertexFunctions()->ConstructVector({x, 0.0f}, {y, 0.0f}, {z, 0.0f}, {w, 0.0f}, {m_outputs[0].ScriptName, glm::vec4(0.0)});
        vmImp->GetFragmentFunctions()->ConstructVector({x, 0.0f}, {y, 0.0f}, {z, 0.0f}, {w, 0.0f}, {m_outputs[0].ScriptName, glm::vec4(0.0)});
        
        return true;
    }

    void SplitVectorNode::OnInit(BlueprintVM* vm)
    {
        this->Title = "Split Vector 4";
        this->Color = glm::vec4(0.5f, 0.5f, 0.5f, 1.0f);
        this->addInput({"Value.xyzw", PinType::Float4, GetInputId()});
        this->addOutput({"Value.x", PinType::Float, GetOutputId()});
        this->addOutput({"Value.y", PinType::Float, GetOutputId()});
        this->addOutput({"Value.z", PinType::Float, GetOutputId()});
        this->addOutput({"Value.w", PinType::Float, GetOutputId()});
    }

    bool SplitVectorNode::Compile(BlueprintVM* vm)
    {
        auto vmImp = static_cast<MaterialVM*>(vm);
        auto link = m_bluePrint->FindLink(m_inputs[0].GetID());
        std::string value = link? link->StartPin.ScriptName: m_inputs[0].ScriptName;
        
        vmImp->GetVertexFunctions()->SplitVector({value, glm::vec4(0.0f)}, {m_outputs[0].ScriptName, 0.0f}, {m_outputs[1].ScriptName, 0.0f}, {m_outputs[2].ScriptName, 0.0f}, {m_outputs[3].ScriptName, glm::vec4(0.0)});
        vmImp->GetFragmentFunctions()->SplitVector({value, glm::vec4(0.0f)}, {m_outputs[0].ScriptName, 0.0f}, {m_outputs[1].ScriptName, 0.0f}, {m_outputs[2].ScriptName, 0.0f}, {m_outputs[3].ScriptName, glm::vec4(0.0)});
        
        return true;
    }

    bool BoolParameterNode::OnGUI()
    {
        bool isChanged = false;
        char buf[128] = {0};
        strncpy(buf, Name.data(), Name.size());
        isChanged |= ImGui::InputText("Name", buf, IM_ARRAYSIZE(buf));
        Name = buf;
        isChanged |= ImGui::Checkbox("Value", &m_value);
        return isChanged;
    }

    void BoolParameterNode::OnInit(BlueprintVM* vm)
    {
        this->Title = "Bool Parameter";
        this->Name = "Default";
        this->Color = glm::vec4(0.0f, 0.5f, 0.0f, 1.0f);
        this->addOutput({"Value.x", PinType::Bool, GetOutputId()});
    }

    bool BoolParameterNode::Compile(BlueprintVM* vm)
    {
        auto vmImp = static_cast<MaterialVM*>(vm);
        vmImp->SetUniform({Name, m_value});
        
        vmImp->GetVertexFunctions()->Assignment({Name, true}, {m_outputs[0].ScriptName, true});
        vmImp->GetFragmentFunctions()->Assignment({Name, true}, {m_outputs[0].ScriptName, true});
        
        return true;
    }

    bool IntParameterNode::OnGUI()
    {
        bool isChanged = false;
        char buf[128] = {0};
        strncpy(buf, Name.data(), Name.size());
        isChanged |= ImGui::InputText("Name", buf, IM_ARRAYSIZE(buf));
        Name = buf;
        isChanged |= ImGui::DragInt("Value", &m_value);
        return isChanged;
    }

    void IntParameterNode::OnInit(BlueprintVM* vm)
    {
        this->Title = "Int Parameter";
        this->Name = "Default";
        this->Color = glm::vec4(0.0f, 0.5f, 0.0f, 1.0f);
        this->addOutput({"Value.x", PinType::Int, GetOutputId()});
    }

    bool IntParameterNode::Compile(BlueprintVM* vm)
    {
        auto vmImp = static_cast<MaterialVM*>(vm);
        vmImp->SetUniform({Name, m_value});
        
        vmImp->GetVertexFunctions()->Assignment({Name, int(0)}, {m_outputs[0].ScriptName, int(0)});
        vmImp->GetFragmentFunctions()->Assignment({Name, int(0)}, {m_outputs[0].ScriptName, int(0)});
        
        return true;
    }

    bool FloatParameterNode::OnGUI()
    {
        bool isChanged = false;
        char buf[128] = {0};
        strncpy(buf, Name.data(), Name.size());
        isChanged |= ImGui::InputText("Name", buf, IM_ARRAYSIZE(buf));
        Name = buf;
        isChanged |= ImGui::DragFloat("Value", &m_value, 0.1f);
        return isChanged;
    }

    void FloatParameterNode::OnInit(BlueprintVM* vm)
    {
        this->Title = "Float Parameter";
        this->Name = "Default";
        this->Color = glm::vec4(0.0f, 0.5f, 0.0f, 1.0f);
        this->addOutput({"Value.x", PinType::Float, GetOutputId()});
    }

    bool FloatParameterNode::Compile(BlueprintVM* vm)
    {
        auto vmImp = static_cast<MaterialVM*>(vm);
        vmImp->SetUniform({Name, m_value});
        
        vmImp->GetVertexFunctions()->Assignment({Name, 0.0f}, {m_outputs[0].ScriptName, 0.0f});
        vmImp->GetFragmentFunctions()->Assignment({Name, 0.0f}, {m_outputs[0].ScriptName, 0.0f});
        
        return true;
    }

    bool Float2ParameterNode::OnGUI()
    {
        bool isChanged = false;
        char buf[128] = {0};
        strncpy(buf, Name.data(), Name.size());
        isChanged |= ImGui::InputText("Name", buf, IM_ARRAYSIZE(buf));
        Name = buf;
        isChanged |= ImGui::DragFloat2("Value", &m_value.x, 0.1f);
        return isChanged;
    }

    void Float2ParameterNode::OnInit(BlueprintVM* vm)
    {
        this->Title = "Float2 Parameter";
        this->Name = "Default";
        this->Color = glm::vec4(0.0f, 0.5f, 0.0f, 1.0f);
        this->addOutput({"Value.xy", PinType::Float2, GetOutputId()});
    }

    bool Float2ParameterNode::Compile(BlueprintVM* vm)
    {
        auto vmImp = static_cast<MaterialVM*>(vm);
        vmImp->SetUniform({Name, m_value});
        
        vmImp->GetVertexFunctions()->Assignment({Name, glm::vec2(0.0)}, {m_outputs[0].ScriptName, glm::vec2(0.0)});
        vmImp->GetFragmentFunctions()->Assignment({Name, glm::vec2(0.0)}, {m_outputs[0].ScriptName, glm::vec2(0.0)});
        
        return true;
    }

    bool Float3ParameterNode::OnGUI()
    {
        bool isChanged = false;
        char buf[128] = {0};
        strncpy(buf, Name.data(), Name.size());
        isChanged |= ImGui::InputText("Name", buf, IM_ARRAYSIZE(buf));
        Name = buf;
        isChanged |= ImGui::DragFloat3("Value", &m_value.x, 0.1f);
        return isChanged;
    }

    void Float3ParameterNode::OnInit(BlueprintVM* vm)
    {
        this->Title = "Float3 Parameter";
        this->Name = "Default";
        this->Color = glm::vec4(0.0f, 0.5f, 0.0f, 1.0f);
        this->addOutput({"Value.xyz", PinType::Float4, GetOutputId()});
    }

    bool Float3ParameterNode::Compile(BlueprintVM* vm)
    {
        auto vmImp = static_cast<MaterialVM*>(vm);
        vmImp->SetUniform({Name, m_value});
        
        vmImp->GetVertexFunctions()->Assignment({Name, glm::vec3(0.0)}, {m_outputs[0].ScriptName, glm::vec3(0.0)});
        vmImp->GetFragmentFunctions()->Assignment({Name, glm::vec3(0.0)}, {m_outputs[0].ScriptName, glm::vec3(0.0)});
        
        return true;
    }

    bool Float4ParameterNode::OnGUI()
    {
        bool isChanged = false;
        char buf[128] = {0};
        strncpy(buf, Name.data(), Name.size());
        isChanged |= ImGui::InputText("Name", buf, IM_ARRAYSIZE(buf));
        Name = buf;
        isChanged |= ImGui::ColorEdit4("Color", &m_value.x);
        return isChanged;
    }

    void Float4ParameterNode::OnInit(BlueprintVM* vm)
    {
        this->Title = "Float4 Parameter";
        this->Name = "Default";
        this->Color = glm::vec4(0.0f, 0.5f, 0.0f, 1.0f);
        this->addOutput({"Value.xyzw", PinType::Float4, GetOutputId()});
    }

    bool Float4ParameterNode::Compile(BlueprintVM* vm)
    {
        auto vmImp = static_cast<MaterialVM*>(vm);
        vmImp->SetUniform({Name, m_value});
        
        vmImp->GetVertexFunctions()->Assignment({Name, glm::vec4(0.0)}, {m_outputs[0].ScriptName, glm::vec4(0.0)});
        vmImp->GetFragmentFunctions()->Assignment({Name, glm::vec4(0.0)}, {m_outputs[0].ScriptName, glm::vec4(0.0)});
        
        return true;
    }

    bool TextureParameterNode::OnGUI()
    {
        bool isChanged = false;
        char buf[128] = {0};
        strncpy(buf, Name.data(), Name.size());
        isChanged |= ImGui::InputText("Name", buf, IM_ARRAYSIZE(buf));
        Name = buf;
        
        std::string name = m_value != nullptr? m_value->GetName(): "None";
        char str[128] = {0};
        strncpy(str, name.c_str(), name.size());
        ImGui::InputText("Texture", str, IM_ARRAYSIZE(str), ImGuiInputTextFlags_ReadOnly);
        if (ImGui::IsItemClicked()) {
            ImGui::OpenPopup("TextureBrowser");
            m_browserWidget = std::shared_ptr<ResourceBrowserWidget>(new ResourceBrowserWidget(Lutra::ResourceManifest::Instance().GetRootDirectoty(), {{".tex", IconType::Texture}, {".rt", IconType::Texture}}, false));
        }

        ImGui::SetNextWindowSize({300, 200});
        if (ImGui::BeginPopupModal("TextureBrowser", NULL, ImGuiWindowFlags_NoResize)) {
            m_browserWidget->OnGUI();
            if (ImGui::Button("OK", ImVec2(50, 0))) {
                m_value = Lutra::ResourceManager::Instance().LoadResource<Lutra::Texture>(m_browserWidget->GetSelectedPath());
                isChanged |= true;
                ImGui::CloseCurrentPopup();
            }
            ImGui::SetItemDefaultFocus();
            ImGui::SameLine();
            if (ImGui::Button("Cancel", ImVec2(50, 0))) {
                ImGui::CloseCurrentPopup();
            }

            ImGui::EndPopup();
        }
        return isChanged;
    }

    void TextureParameterNode::OnInit(BlueprintVM* vm)
    {
        this->Title = "Texture Parameter";
        this->Name = "Default";
        this->Color = glm::vec4(0.0f, 0.5f, 0.0f, 1.0f);
        this->addInput({"Texcoord.xy", PinType::Float2, GetInputId()});
        this->addOutput({"Value.rgba", PinType::Float4, GetOutputId()});
    }

    bool TextureParameterNode::Compile(BlueprintVM* vm)
    {
        auto vmImp = static_cast<MaterialVM*>(vm);
        vmImp->SetUniform({Name, m_value});
        
        std::string name = findLinkName(m_inputs[0]);
        vmImp->GetFragmentFunctions()->Assignment({"v_texcoord0", glm::vec2(0.0)}, {m_inputs[0].ScriptName, glm::vec2(0.0)});
        vmImp->GetFragmentFunctions()->TextureFetch({Name, m_value}, {name, glm::vec2(0.0)}, {m_outputs[0].ScriptName, glm::vec4(0.0)});
        return true;
    }

    void SurfacePositionNode::OnInit(BlueprintVM* vm)
    {
        this->Title = "Surface Position";
        this->Color = glm::vec4(0.0f, 0.0f, 0.5f, 1.0f);
        this->addOutput({"(World Space)Position.xyz", PinType::Float3, GetOutputId()});
        
    }

    bool SurfacePositionNode::Compile(BlueprintVM* vm)
    {
        auto vmImp = static_cast<MaterialVM*>(vm);
        vmImp->GetVertexFunctions()->Assignment({"v_worldPosition", glm::vec3(0.0)}, {m_outputs[0].ScriptName, glm::vec3(0.0)});
        vmImp->GetFragmentFunctions()->Assignment({"v_worldPosition", glm::vec3(0.0)}, {m_outputs[0].ScriptName, glm::vec3(0.0)});
        return true;
    }

    void SurfaceNormalNode::OnInit(BlueprintVM* vm)
    {
        this->Title = "Surface Normal";
        this->Color = glm::vec4(0.0f, 0.0f, 0.5f, 1.0f);
        this->addOutput({"(World Space)Normal.xyz", PinType::Float3, GetOutputId()});
    }

    bool SurfaceNormalNode::Compile(BlueprintVM* vm)
    {
        auto vmImp = static_cast<MaterialVM*>(vm);
        vmImp->GetVertexFunctions()->Assignment({"v_worldNormal", glm::vec3(0.0)}, {m_outputs[0].ScriptName, glm::vec3(0.0)});
        vmImp->GetFragmentFunctions()->Assignment({"v_worldNormal", glm::vec3(0.0)}, {m_outputs[0].ScriptName, glm::vec3(0.0)});
        return true;
    }

    void SurfaceTangentNode::OnInit(BlueprintVM* vm)
    {
        this->Title = "Surface Tangent";
        this->Color = glm::vec4(0.0f, 0.0f, 0.5f, 1.0f);
        this->addOutput({"(World Space)Tangent.xyz", PinType::Float4, GetOutputId()});
    }

    bool SurfaceTangentNode::Compile(BlueprintVM* vm)
    {
        auto vmImp = static_cast<MaterialVM*>(vm);
        vmImp->GetVertexFunctions()->Assignment({"v_worldTangent", glm::vec3(0.0)}, {m_outputs[0].ScriptName, glm::vec3(0.0)});
        vmImp->GetFragmentFunctions()->Assignment({"v_worldTangent", glm::vec3(0.0)}, {m_outputs[0].ScriptName, glm::vec3(0.0)});
        return true;
    }

    void SurfaceColorNode::OnInit(BlueprintVM* vm)
    {
        this->Title = "Surface Color";
        this->Color = glm::vec4(0.0f, 0.0f, 0.5f, 1.0f);
        this->addOutput({"Color.rgba", PinType::Float4, GetOutputId()});
    }

    bool SurfaceColorNode::Compile(BlueprintVM* vm)
    {
        auto vmImp = static_cast<MaterialVM*>(vm);
        vmImp->GetVertexFunctions()->Assignment({"v_color", glm::vec4(0.0)}, {m_outputs[0].ScriptName, glm::vec4(0.0)});
        vmImp->GetFragmentFunctions()->Assignment({"v_color", glm::vec4(0.0)}, {m_outputs[0].ScriptName, glm::vec4(0.0)});
        return true;
    }

    void SurfaceTexcoord0Node::OnInit(BlueprintVM* vm)
    {
        this->Title = "Surface Texcoord0";
        this->Color = glm::vec4(0.0f, 0.0f, 0.5f, 1.0f);
        this->addOutput({"Texcoord0.xy", PinType::Float2, GetOutputId()});
    }

    bool SurfaceTexcoord0Node::Compile(BlueprintVM* vm)
    {
        auto vmImp = static_cast<MaterialVM*>(vm);
        vmImp->GetVertexFunctions()->Assignment({"v_texcoord0", glm::vec2(0.0)}, {m_outputs[0].ScriptName, glm::vec2(0.0)});
        vmImp->GetFragmentFunctions()->Assignment({"v_texcoord0", glm::vec2(0.0)}, {m_outputs[0].ScriptName, glm::vec2(0.0)});
        return true;
    }

    void SurfaceTexcoord1Node::OnInit(BlueprintVM* vm)
    {
        this->Title = "Surface Texcoord1";
        this->Color = glm::vec4(0.0f, 0.0f, 0.5f, 1.0f);
        this->addOutput({"Texcoord1.xy", PinType::Float2, GetOutputId()});
    }

    bool SurfaceTexcoord1Node::Compile(BlueprintVM* vm)
    {
        auto vmImp = static_cast<MaterialVM*>(vm);
        vmImp->GetVertexFunctions()->Assignment({"v_texcoord1", glm::vec2(0.0)}, {m_outputs[0].ScriptName, glm::vec2(0.0)});
        vmImp->GetFragmentFunctions()->Assignment({"v_texcoord1", glm::vec2(0.0)}, {m_outputs[0].ScriptName, glm::vec2(0.0)});
        return true;
    }

    void AbsNode::OnInit(BlueprintVM* vm)
    {
        this->Title = "Abs";
        this->Color = glm::vec4(0.5f, 0.5f, 0.5f, 1.0f);
        this->addInput({"A.xyzw", PinType::Float4, GetInputId()});
        this->addOutput({"abs(A).xyzw", PinType::Float4, GetOutputId()});
    }

    bool AbsNode::Compile(BlueprintVM* vm)
    {
        auto vmImp = static_cast<MaterialVM*>(vm);
        std::string name = findLinkName(m_inputs[0]);
        vmImp->GetVertexFunctions()->Abs({name, glm::vec4(0.0f)}, {m_outputs[0].ScriptName, glm::vec4(0.0f)});
        vmImp->GetFragmentFunctions()->Abs({name, glm::vec4(0.0f)}, {m_outputs[0].ScriptName, glm::vec4(0.0f)});
        return true;
    }

    void AddNode::OnInit(BlueprintVM* vm)
    {
        this->Title = "Add";
        this->Color = glm::vec4(0.5f, 0.5f, 0.5f, 1.0f);
        this->addInput({"A.xyzw", PinType::Float4, GetInputId()});
        this->addInput({"B.xyzw", PinType::Float4, GetInputId()});
        this->addOutput({"(A + B).xyzw", PinType::Float4, GetOutputId()});
    }

    bool AddNode::Compile(BlueprintVM* vm)
    {
        auto vmImp = static_cast<MaterialVM*>(vm);
        std::string AName = findLinkName(m_inputs[0]);
        std::string BName = findLinkName(m_inputs[1]);
        vmImp->GetVertexFunctions()->Add({AName, glm::vec4(0.0f)}, {BName, glm::vec4(0.0f)}, {m_outputs[0].ScriptName, glm::vec4(0.0f)});
        vmImp->GetFragmentFunctions()->Add({AName, glm::vec4(0.0f)}, {BName, glm::vec4(0.0f)}, {m_outputs[0].ScriptName, glm::vec4(0.0f)});
        return true;
    }

    void CeilNode::OnInit(BlueprintVM* vm)
    {
        this->Title = "Ceil";
        this->Color = glm::vec4(0.5f, 0.5f, 0.5f, 1.0f);
        this->addInput({"A.xyzw", PinType::Float4, GetInputId()});
        this->addOutput({"ceil(A).xyzw", PinType::Float4, GetOutputId()});
    }

    bool CeilNode::Compile(BlueprintVM* vm)
    {
        auto vmImp = static_cast<MaterialVM*>(vm);
        std::string AName = findLinkName(m_inputs[0]);
        vmImp->GetVertexFunctions()->Ceil({AName, glm::vec4(0.0f)}, {m_outputs[0].ScriptName, glm::vec4(0.0f)});
        vmImp->GetFragmentFunctions()->Ceil({AName, glm::vec4(0.0f)}, {m_outputs[0].ScriptName, glm::vec4(0.0f)});
        return true;
    }

    void ClampNode::OnInit(BlueprintVM* vm)
    {
        this->Title = "Clamp";
        this->Color = glm::vec4(0.5f, 0.5f, 0.5f, 1.0f);
        this->addInput({"A.xyzw", PinType::Float4, GetInputId()});
        this->addInput({"Min.xyzw", PinType::Float4, GetInputId()});
        this->addInput({"Max.xyzw", PinType::Float4, GetInputId()});
        this->addOutput({"clamp(A, Min, Max).xyzw", PinType::Float4, GetOutputId()});
    }

    bool ClampNode::Compile(BlueprintVM* vm)
    {
        auto vmImp = static_cast<MaterialVM*>(vm);
        std::string AName = findLinkName(m_inputs[0]);
        std::string MinName = findLinkName(m_inputs[1]);
        std::string MaxName = findLinkName(m_inputs[2]);
        vmImp->GetVertexFunctions()->Clamp({AName, glm::vec4(0.0f)}, {MinName, glm::vec4(0.0f)}, {MaxName, glm::vec4(0.0f)}, {m_outputs[0].ScriptName, glm::vec4(0.0f)});
        vmImp->GetFragmentFunctions()->Clamp({AName, glm::vec4(0.0f)}, {MinName, glm::vec4(0.0f)}, {MaxName, glm::vec4(0.0f)}, {m_outputs[0].ScriptName, glm::vec4(0.0f)});
        return true;
    }

    void DivideNode::OnInit(BlueprintVM* vm)
    {
        this->Title = "Divide";
        this->Color = glm::vec4(0.5f, 0.5f, 0.5f, 1.0f);
        this->addInput({"A.xyzw", PinType::Float4, GetInputId()});
        this->addInput({"B.xyzw", PinType::Float4, GetInputId()});
        this->addOutput({"(A / B).xyzw", PinType::Float4, GetOutputId()});
    }

    bool DivideNode::Compile(BlueprintVM* vm)
    {
        auto vmImp = static_cast<MaterialVM*>(vm);
        std::string AName = findLinkName(m_inputs[0]);
        std::string BName = findLinkName(m_inputs[1]);
        vmImp->GetVertexFunctions()->Divide({AName, glm::vec4(0.0f)}, {BName, glm::vec4(1.0f)}, {m_outputs[0].ScriptName, glm::vec4(1.0f)});
        vmImp->GetFragmentFunctions()->Divide({AName, glm::vec4(0.0f)}, {BName, glm::vec4(1.0f)}, {m_outputs[0].ScriptName, glm::vec4(1.0f)});
        return true;
    }

    void DDXNode::OnInit(BlueprintVM* vm)
    {
        this->Title = "DDX";
        this->Color = glm::vec4(0.5f, 0.5f, 0.5f, 1.0f);
        this->addInput({"A.xyzw", PinType::Float4, GetInputId()});
        this->addOutput({"ddx(A).xyzw", PinType::Float4, GetOutputId()});
    }

    bool DDXNode::Compile(BlueprintVM* vm)
    {
        auto vmImp = static_cast<MaterialVM*>(vm);
        std::string AName = findLinkName(m_inputs[0]);
        vmImp->GetFragmentFunctions()->DDX({AName, glm::vec4(0.0f)}, {m_outputs[0].ScriptName, glm::vec4(0.0f)});
        return true;
    }

    void DDYNode::OnInit(BlueprintVM* vm)
    {
        this->Title = "DDY";
        this->Color = glm::vec4(0.5f, 0.5f, 0.5f, 1.0f);
        this->addInput({"A.xyzw", PinType::Float4, GetInputId()});
        this->addOutput({"ddy(A).xyzw", PinType::Float4, GetOutputId()});
    }

    bool DDYNode::Compile(BlueprintVM* vm)
    {
        auto vmImp = static_cast<MaterialVM*>(vm);
        std::string AName = findLinkName(m_inputs[0]);
        vmImp->GetFragmentFunctions()->DDY({AName, glm::vec4(0.0f)}, {m_outputs[0].ScriptName, glm::vec4(0.0f)});
        return true;
    }

    void DiscardNode::OnInit(BlueprintVM* vm)
    {
        this->Title = "Discard";
        this->Color = glm::vec4(0.5f, 0.5f, 0.5f, 1.0f);
        this->addInput({"A.xyzw", PinType::Float4, GetInputId()});
        this->addInput({"Bool.x", PinType::Bool, GetInputId()});
        this->addOutput({"A.xyzw", PinType::Float4, GetOutputId()});
    }

    bool DiscardNode::Compile(BlueprintVM* vm)
    {
        auto vmImp = static_cast<MaterialVM*>(vm);
        std::string AName = findLinkName(m_inputs[0]);
        std::string BoolName = findLinkName(m_inputs[1]);
        vmImp->GetFragmentFunctions()->Discard({AName, glm::vec4(0.0f)}, {BoolName, true}, {m_outputs[0].ScriptName, glm::vec4(0.0f)});
        return true;
    }

    void FloorNode::OnInit(BlueprintVM* vm)
    {
        this->Title = "Floor";
        this->Color = glm::vec4(0.5f, 0.5f, 0.5f, 1.0f);
        this->addInput({"A.xyzw", PinType::Float4, GetInputId()});
        this->addOutput({"floor(A).xyzw", PinType::Float4, GetOutputId()});
    }

    bool FloorNode::Compile(BlueprintVM* vm)
    {
        auto vmImp = static_cast<MaterialVM*>(vm);
        std::string AName = findLinkName(m_inputs[0]);
        vmImp->GetVertexFunctions()->Floor({AName, glm::vec4(0.0f)}, {m_outputs[0].ScriptName, glm::vec4(0.0f)});
        vmImp->GetFragmentFunctions()->Floor({AName, glm::vec4(0.0f)}, {m_outputs[0].ScriptName, glm::vec4(0.0f)});
        return true;
    }

    void FractNode::OnInit(BlueprintVM* vm)
    {
        this->Title = "Fract";
        this->Color = glm::vec4(0.5f, 0.5f, 0.5f, 1.0f);
        this->addInput({"A.xyzw", PinType::Float4, GetInputId()});
        this->addOutput({"fract(A).xyzw", PinType::Float4, GetOutputId()});
    }

    bool FractNode::Compile(BlueprintVM* vm)
    {
        auto vmImp = static_cast<MaterialVM*>(vm);
        std::string AName = findLinkName(m_inputs[0]);
        vmImp->GetVertexFunctions()->Fract({AName, glm::vec4(0.0f)}, {m_outputs[0].ScriptName, glm::vec4(0.0f)});
        vmImp->GetFragmentFunctions()->Fract({AName, glm::vec4(0.0f)}, {m_outputs[0].ScriptName, glm::vec4(0.0f)});
        return true;
    }

    void ReciprocalNode::OnInit(BlueprintVM* vm)
    {
        this->Title = "Reciprocal";
        this->Color = glm::vec4(0.5f, 0.5f, 0.5f, 1.0f);
        this->addInput({"A.xyzw", PinType::Float4, GetInputId()});
        this->addOutput({"(1 / A).xyzw", PinType::Float4, GetOutputId()});
    }

    bool ReciprocalNode::Compile(BlueprintVM* vm)
    {
        auto vmImp = static_cast<MaterialVM*>(vm);
        std::string AName = findLinkName(m_inputs[0]);
        
        vmImp->GetVertexFunctions()->Divide({"One", 1.0f}, {AName, glm::vec4(1.0)}, {m_outputs[0].ScriptName, glm::vec4(0.0f)});
        vmImp->GetFragmentFunctions()->Divide({"One", 1.0f}, {AName, glm::vec4(1.0)}, {m_outputs[0].ScriptName, glm::vec4(0.0f)});
        return true;
    }

    void MaxNode::OnInit(BlueprintVM* vm)
    {
        this->Title = "Max";
        this->Color = glm::vec4(0.5f, 0.5f, 0.5f, 1.0f);
        this->addInput({"A.xyzw", PinType::Float4, GetInputId()});
        this->addInput({"B.xyzw", PinType::Float4, GetInputId()});
        this->addOutput({"max(A, B).xyzw", PinType::Float4, GetOutputId()});
    }

    bool MaxNode::Compile(BlueprintVM* vm)
    {
        auto vmImp = static_cast<MaterialVM*>(vm);
        std::string AName = findLinkName(m_inputs[0]);
        std::string BName = findLinkName(m_inputs[1]);
        
        vmImp->GetVertexFunctions()->Max({AName, glm::vec4(0.0)}, {BName, glm::vec4(0.0)}, {m_outputs[0].ScriptName, glm::vec4(0.0f)});
        vmImp->GetFragmentFunctions()->Max({AName, glm::vec4(0.0)}, {BName, glm::vec4(0.0)}, {m_outputs[0].ScriptName, glm::vec4(0.0f)});
        return true;
    }

    void MinNode::OnInit(BlueprintVM* vm)
    {
        this->Title = "Min";
        this->Color = glm::vec4(0.5f, 0.5f, 0.5f, 1.0f);
        this->addInput({"A.xyzw", PinType::Float4, GetInputId()});
        this->addInput({"B.xyzw", PinType::Float4, GetInputId()});
        this->addOutput({"min(A, B).xyzw", PinType::Float4, GetOutputId()});
    }

    bool MinNode::Compile(BlueprintVM* vm)
    {
        auto vmImp = static_cast<MaterialVM*>(vm);
        std::string AName = findLinkName(m_inputs[0]);
        std::string BName = findLinkName(m_inputs[1]);
        
        vmImp->GetVertexFunctions()->Min({AName, glm::vec4(0.0)}, {BName, glm::vec4(0.0)}, {m_outputs[0].ScriptName, glm::vec4(0.0f)});
        vmImp->GetFragmentFunctions()->Min({AName, glm::vec4(0.0)}, {BName, glm::vec4(0.0)}, {m_outputs[0].ScriptName, glm::vec4(0.0f)});
        return true;
    }

    void MixNode::OnInit(BlueprintVM* vm)
    {
        this->Title = "Mix";
        this->Color = glm::vec4(0.5f, 0.5f, 0.5f, 1.0f);
        this->addInput({"A.xyzw", PinType::Float4, GetInputId()});
        this->addInput({"B.xyzw", PinType::Float4, GetInputId()});
        this->addInput({"Alpha.xyzw", PinType::Float4, GetInputId()});
        this->addOutput({"mix(A, B, Alpha).xyzw", PinType::Float4, GetOutputId()});
    }

    bool MixNode::Compile(BlueprintVM* vm)
    {
        auto vmImp = static_cast<MaterialVM*>(vm);
        std::string AName = findLinkName(m_inputs[0]);
        std::string BName = findLinkName(m_inputs[1]);
        std::string AlphaName = findLinkName(m_inputs[2]);
        
        vmImp->GetVertexFunctions()->Mix({AName, glm::vec4(0.0)}, {BName, glm::vec4(0.0)}, {AlphaName, glm::vec4(0.0)}, {m_outputs[0].ScriptName, glm::vec4(0.0f)});
        vmImp->GetFragmentFunctions()->Mix({AName, glm::vec4(0.0)}, {BName, glm::vec4(0.0)}, {AlphaName, glm::vec4(0.0)}, {m_outputs[0].ScriptName, glm::vec4(0.0f)});
        return true;
    }

    void ModNode::OnInit(BlueprintVM* vm)
    {
        this->Title = "Mod";
        this->Color = glm::vec4(0.5f, 0.5f, 0.5f, 1.0f);
        this->addInput({"A.xyzw", PinType::Float4, GetInputId()});
        this->addInput({"Divider.xyzw", PinType::Float4, GetInputId()});
        this->addOutput({"mod(A, Divider).xyzw", PinType::Float4, GetOutputId()});
    }

    bool ModNode::Compile(BlueprintVM* vm)
    {
        auto vmImp = static_cast<MaterialVM*>(vm);
        std::string AName = findLinkName(m_inputs[0]);
        std::string BName = findLinkName(m_inputs[1]);
        
        vmImp->GetVertexFunctions()->Mod({AName, glm::vec4(0.0)}, {BName, glm::vec4(0.0)}, {m_outputs[0].ScriptName, glm::vec4(0.0f)});
        vmImp->GetFragmentFunctions()->Mod({AName, glm::vec4(0.0)}, {BName, glm::vec4(0.0)}, {m_outputs[0].ScriptName, glm::vec4(0.0f)});
        return true;
    }

    void MultiplyNode::OnInit(BlueprintVM* vm)
    {
        this->Title = "Multiply";
        this->Color = glm::vec4(0.5f, 0.5f, 0.5f, 1.0f);
        this->addInput({"A.xyzw", PinType::Float4, GetInputId()});
        this->addInput({"B.xyzw", PinType::Float4, GetInputId()});
        this->addOutput({"(A * B).xyzw", PinType::Float4, GetOutputId()});
    }

    bool MultiplyNode::Compile(BlueprintVM* vm)
    {
        auto vmImp = static_cast<MaterialVM*>(vm);
        std::string AName = findLinkName(m_inputs[0]);
        std::string BName = findLinkName(m_inputs[1]);
        
        vmImp->GetVertexFunctions()->Multiply({AName, glm::vec4(0.0)}, {BName, glm::vec4(0.0)}, {m_outputs[0].ScriptName, glm::vec4(0.0f)});
        vmImp->GetFragmentFunctions()->Multiply({AName, glm::vec4(0.0)}, {BName, glm::vec4(0.0)}, {m_outputs[0].ScriptName, glm::vec4(0.0f)});
        return true;
    }

    void NegateNode::OnInit(BlueprintVM* vm)
    {
        this->Title = "Negate";
        this->Color = glm::vec4(0.5f, 0.5f, 0.5f, 1.0f);
        this->addInput({"A.xyzw", PinType::Float4, GetInputId()});
        this->addOutput({"-A.xyzw", PinType::Float4, GetOutputId()});
    }

    bool NegateNode::Compile(BlueprintVM* vm)
    {
        auto vmImp = static_cast<MaterialVM*>(vm);
        std::string AName = findLinkName(m_inputs[0]);
        
        vmImp->GetVertexFunctions()->Negate({AName, glm::vec4(0.0)}, {m_outputs[0].ScriptName, glm::vec4(0.0f)});
        vmImp->GetFragmentFunctions()->Negate({AName, glm::vec4(0.0)}, {m_outputs[0].ScriptName, glm::vec4(0.0f)});
        return true;
    }

    void OneMinusNode::OnInit(BlueprintVM* vm)
    {
        this->Title = "OneMinus";
        this->Color = glm::vec4(0.5f, 0.5f, 0.5f, 1.0f);
        this->addInput({"A.xyzw", PinType::Float4, GetInputId()});
        this->addOutput({"(1 - A).xyzw", PinType::Float4, GetOutputId()});
    }

    bool OneMinusNode::Compile(BlueprintVM* vm)
    {
        auto vmImp = static_cast<MaterialVM*>(vm);
        std::string AName = findLinkName(m_inputs[0]);
        vmImp->GetVertexFunctions()->Subtract({"One", 1.0f}, {AName, glm::vec4(0.0)}, {m_outputs[0].ScriptName, glm::vec4(0.0f)});
        vmImp->GetFragmentFunctions()->Subtract({"One", 1.0f}, {AName, glm::vec4(0.0)}, {m_outputs[0].ScriptName, glm::vec4(0.0f)});
        return true;
    }

    void RoundNode::OnInit(BlueprintVM* vm)
    {
        this->Title = "Round";
        this->Color = glm::vec4(0.5f, 0.5f, 0.5f, 1.0f);
        this->addInput({"A.xyzw", PinType::Float4, GetInputId()});
        this->addOutput({"floor(A + 0.5).xyzw", PinType::Float4, GetOutputId()});
    }

    bool RoundNode::Compile(BlueprintVM* vm)
    {
        auto vmImp = static_cast<MaterialVM*>(vm);
        std::string AName = findLinkName(m_inputs[0]);
        vmImp->GetVertexFunctions()->Round({AName, glm::vec4(0.0)}, {m_outputs[0].ScriptName, glm::vec4(0.0f)});
        vmImp->GetFragmentFunctions()->Round({AName, glm::vec4(0.0)}, {m_outputs[0].ScriptName, glm::vec4(0.0f)});
        return true;
    }

    void ScaleAndOffsetNode::OnInit(BlueprintVM* vm)
    {
        this->Title = "Scale And Offset";
        this->Color = glm::vec4(0.5f, 0.5f, 0.5f, 1.0f);
        this->addInput({"A.xyzw", PinType::Float4, GetInputId()});
        this->addInput({"Scale.xyzw", PinType::Float4, GetInputId()});
        this->addInput({"Offset.xyzw", PinType::Float4, GetInputId()});
        this->addOutput({"(A * Scale + Offset).xyzw", PinType::Float4, GetOutputId()});
    }

    bool ScaleAndOffsetNode::Compile(BlueprintVM* vm)
    {
        auto vmImp = static_cast<MaterialVM*>(vm);
        std::string AName = findLinkName(m_inputs[0]);
        std::string ScaleName = findLinkName(m_inputs[1]);
        std::string OffsetName = findLinkName(m_inputs[2]);
        
        vmImp->GetVertexFunctions()->Multiply({AName, glm::vec4(0.0f)}, {ScaleName, glm::vec4(0.0f)}, {m_outputs[0].ScriptName, glm::vec4(0.0f)});
        vmImp->GetVertexFunctions()->Add({m_outputs[0].ScriptName, glm::vec4(0.0f)}, {OffsetName, glm::vec4(0.0f)}, {m_outputs[0].ScriptName, glm::vec4(0.0f)});
        vmImp->GetFragmentFunctions()->Multiply({AName, glm::vec4(0.0f)}, {ScaleName, glm::vec4(0.0f)}, {m_outputs[0].ScriptName, glm::vec4(0.0f)});
        vmImp->GetFragmentFunctions()->Add({m_outputs[0].ScriptName, glm::vec4(0.0f)}, {OffsetName, glm::vec4(0.0f)}, {m_outputs[0].ScriptName, glm::vec4(0.0f)});
        return true;
    }

    void SignNode::OnInit(BlueprintVM* vm)
    {
        this->Title = "Sign";
        this->Color = glm::vec4(0.5f, 0.5f, 0.5f, 1.0f);
        this->addInput({"A.xyzw", PinType::Float4, GetInputId()});
        this->addOutput({"sign(A).xyzw", PinType::Float4, GetOutputId()});
    }

    bool SignNode::Compile(BlueprintVM* vm)
    {
        auto vmImp = static_cast<MaterialVM*>(vm);
        std::string AName = findLinkName(m_inputs[0]);
        vmImp->GetVertexFunctions()->Sign({AName, glm::vec4(0.0)}, {m_outputs[0].ScriptName, glm::vec4(0.0f)});
        vmImp->GetFragmentFunctions()->Sign({AName, glm::vec4(0.0)}, {m_outputs[0].ScriptName, glm::vec4(0.0f)});
        return true;
    }

    void SmoothStepNode::OnInit(BlueprintVM* vm)
    {
        this->Title = "SmoothStep";
        this->Color = glm::vec4(0.5f, 0.5f, 0.5f, 1.0f);
        this->addInput({"A.xyzw", PinType::Float4, GetInputId()});
        this->addInput({"B.xyzw", PinType::Float4, GetInputId()});
        this->addInput({"Alpha.xyzw", PinType::Float4, GetInputId()});
        this->addOutput({"smoothStep(A, B, Alpha).xyzw", PinType::Float4, GetOutputId()});
    }

    bool SmoothStepNode::Compile(BlueprintVM* vm)
    {
        auto vmImp = static_cast<MaterialVM*>(vm);
        std::string AName = findLinkName(m_inputs[0]);
        std::string BName = findLinkName(m_inputs[1]);
        std::string AlphaName = findLinkName(m_inputs[2]);
        
        vmImp->GetVertexFunctions()->SmoothStep({AName, glm::vec4(0.0)}, {BName, glm::vec4(0.0)}, {AlphaName, glm::vec4(0.0)}, {m_outputs[0].ScriptName, glm::vec4(0.0f)});
        vmImp->GetFragmentFunctions()->SmoothStep({AName, glm::vec4(0.0)}, {BName, glm::vec4(0.0)}, {AlphaName, glm::vec4(0.0)}, {m_outputs[0].ScriptName, glm::vec4(0.0f)});
        return true;
    }

    void StepNode::OnInit(BlueprintVM* vm)
    {
        this->Title = "Step";
        this->Color = glm::vec4(0.5f, 0.5f, 0.5f, 1.0f);
        this->addInput({"A.xyzw", PinType::Float4, GetInputId()});
        this->addInput({"B.xyzw", PinType::Float4, GetInputId()});
        this->addOutput({"step(A, B).xyzw", PinType::Float4, GetOutputId()});
    }

    bool StepNode::Compile(BlueprintVM* vm)
    {
        auto vmImp = static_cast<MaterialVM*>(vm);
        std::string AName = findLinkName(m_inputs[0]);
        std::string BName = findLinkName(m_inputs[1]);
        
        vmImp->GetVertexFunctions()->Step({AName, glm::vec4(0.0)}, {BName, glm::vec4(0.0)}, {m_outputs[0].ScriptName, glm::vec4(0.0f)});
        vmImp->GetFragmentFunctions()->Step({AName, glm::vec4(0.0)}, {BName, glm::vec4(0.0)}, {m_outputs[0].ScriptName, glm::vec4(0.0f)});
        return true;
    }

    void SubtractNode::OnInit(BlueprintVM* vm)
    {
        this->Title = "Subtract";
        this->Color = glm::vec4(0.5f, 0.5f, 0.5f, 1.0f);
        this->addInput({"A.xyzw", PinType::Float4, GetInputId()});
        this->addInput({"B.xyzw", PinType::Float4, GetInputId()});
        this->addOutput({"(A - B).xyzw", PinType::Float4, GetOutputId()});
    }

    bool SubtractNode::Compile(BlueprintVM* vm)
    {
        auto vmImp = static_cast<MaterialVM*>(vm);
        std::string AName = findLinkName(m_inputs[0]);
        std::string BName = findLinkName(m_inputs[1]);
        
        vmImp->GetVertexFunctions()->Subtract({AName, glm::vec4(0.0)}, {BName, glm::vec4(0.0)}, {m_outputs[0].ScriptName, glm::vec4(0.0f)});
        vmImp->GetFragmentFunctions()->Subtract({AName, glm::vec4(0.0)}, {BName, glm::vec4(0.0)}, {m_outputs[0].ScriptName, glm::vec4(0.0f)});
        return true;
    }

    void ScaleCoordsNode::OnInit(BlueprintVM* vm)
    {
        this->Title = "Scale Coords";
        this->Color = glm::vec4(0.5f, 0.5f, 0.5f, 1.0f);
        this->addInput({"Texcoord.xy", PinType::Float2, GetInputId()});
        this->addInput({"Scale.xy", PinType::Float2, GetInputId()});
        this->addInput({"Center.xy", PinType::Float2, GetInputId()});
        this->addOutput({"Texcoord.xy", PinType::Float2, GetOutputId()});
        
//        static_cast<MaterialVM*>(vm)->Bind("ScaleCoords", [](){
//            return R"(
//            void ScaleCoords(in vec4 Texcoord, in vec4 Scale, in vec4 Center, out vec4 outVal) {
//                outVal = (Texcoord / Scale + Center) - (Center / Scale);
//            }
//            )";
//        }, MaterialVM::BindDomain::Vertex | MaterialVM::BindDomain::Fragment);
    }

    bool ScaleCoordsNode::Compile(BlueprintVM* vm)
    {
        auto vmImp = static_cast<MaterialVM*>(vm);
        std::string AName = findLinkName(m_inputs[0]);
        std::string ScaleName = findLinkName(m_inputs[1]);
        std::string CenterName = findLinkName(m_inputs[2]);
        
        vmImp->GetVertexFunctions()->Divide({AName, glm::vec2(0.0)}, {ScaleName, glm::vec2(1.0)}, {"ScaleCoords_Temp", glm::vec2(0.0f)});
        vmImp->GetVertexFunctions()->Add({"ScaleCoords_Temp", glm::vec4(0.0f)}, {CenterName, glm::vec2(0.5f)}, {"ScaleCoords_Temp", glm::vec4(0.0f)});
        vmImp->GetVertexFunctions()->Divide({CenterName, glm::vec2(0.0)}, {ScaleName, glm::vec2(1.0)}, {"ScaleCoords_Temp1", glm::vec2(0.0f)});
        vmImp->GetVertexFunctions()->Subtract({"ScaleCoords_Temp", glm::vec2(0.0)}, {"ScaleCoords_Temp1", glm::vec2(0.0)}, {m_outputs[0].ScriptName, glm::vec2(0.0f)});
        
        vmImp->GetFragmentFunctions()->Divide({AName, glm::vec2(0.0)}, {ScaleName, glm::vec2(1.0)}, {"ScaleCoords_Temp", glm::vec2(0.0f)});
        vmImp->GetFragmentFunctions()->Add({"ScaleCoords_Temp", glm::vec4(0.0f)}, {CenterName, glm::vec2(0.5f)}, {"ScaleCoords_Temp", glm::vec4(0.0f)});
        vmImp->GetFragmentFunctions()->Divide({CenterName, glm::vec2(0.0)}, {ScaleName, glm::vec2(1.0)}, {"ScaleCoords_Temp1", glm::vec2(0.0f)});
        vmImp->GetFragmentFunctions()->Subtract({"ScaleCoords_Temp", glm::vec2(0.0)}, {"ScaleCoords_Temp1", glm::vec2(0.0)}, {m_outputs[0].ScriptName, glm::vec2(0.0f)});
        
        return true;
    }

    void DeltaTimeNode::OnInit(BlueprintVM* vm)
    {
        this->Title = "Delta Time";
        this->Color = glm::vec4(0.5f, 0.5f, 0.5f, 1.0f);
        this->addInput({"Multiplier.x", PinType::Float, GetInputId()});
        this->addOutput({"Time.x", PinType::Float, GetOutputId()});
    }

    bool DeltaTimeNode::Compile(BlueprintVM* vm)
    {
        auto vmImp = static_cast<MaterialVM*>(vm);
        vmImp->SetUniform({"u_DeltaTime", 0.0f});
        std::string AName = findLinkName(m_inputs[0]);
        
        vmImp->GetVertexFunctions()->Multiply({AName, 0.0f}, {"u_DeltaTime", 0.0f}, {m_outputs[0].ScriptName, glm::vec2(0.0f)});
        vmImp->GetFragmentFunctions()->Multiply({AName, 0.0f}, {"u_DeltaTime", 0.0f}, {m_outputs[0].ScriptName, glm::vec2(0.0f)});
        
        return true;
    }

    void ElapsedTimeNode::OnInit(BlueprintVM* vm)
    {
        this->Title = "Elapsed Time";
        this->Color = glm::vec4(0.5f, 0.5f, 0.5f, 1.0f);
        this->addInput({"Multiplier.x", PinType::Float, GetInputId()});
        this->addOutput({"Time.x", PinType::Float, GetOutputId()});
    }

    bool ElapsedTimeNode::Compile(BlueprintVM* vm)
    {
        auto vmImp = static_cast<MaterialVM*>(vm);
        vmImp->SetUniform({"u_ElapsedTime", 0.0f});
        std::string AName = findLinkName(m_inputs[0]);
        
        vmImp->GetVertexFunctions()->Multiply({AName, 0.0f}, {"u_ElapsedTime", 0.0f}, {m_outputs[0].ScriptName, glm::vec2(0.0f)});
        vmImp->GetFragmentFunctions()->Multiply({AName, 0.0f}, {"u_ElapsedTime", 0.0f}, {m_outputs[0].ScriptName, glm::vec2(0.0f)});
        
        return true;
    }

}
