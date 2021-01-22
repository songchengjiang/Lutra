//
//  BlueprintVM.cpp
//  Editor
//
//  Created by JasonCheng on 2021/1/19.
//

#include "BlueprintVM.h"
#include <set>

namespace LutraEditor {

    std::string GetValueType(VarientType type)
    {
        switch (type) {
            case VarientType::Bool: return "bool";
            case VarientType::Int: return "Int";
            case VarientType::Float: return "float";
            case VarientType::Float2: return "vec2";
            case VarientType::Float3: return "vec3";
            case VarientType::Float4: return "vec4";
            case VarientType::Sampler: return "sampler2D";
        }
        return "";
    }

    std::string GetValue(Varient var)
    {
        switch (var.GetType()) {
            case VarientType::Bool: return var.AsBool()? "true": "false";
            case VarientType::Int: return std::to_string(var.AsInt());
            case VarientType::Float: return std::to_string(var.AsFloat());
            case VarientType::Float2: return "vec2(" + std::to_string(var.AsFloat2().x) + "," + std::to_string(var.AsFloat2().y) + ")";
            case VarientType::Float3: return "vec3(" + std::to_string(var.AsFloat3().x) + "," + std::to_string(var.AsFloat3().y) + "," + std::to_string(var.AsFloat3().z) + ")";
            case VarientType::Float4: return "vec4(" + std::to_string(var.AsFloat4().x) + "," + std::to_string(var.AsFloat4().y) + "," + std::to_string(var.AsFloat4().z) + "," + std::to_string(var.AsFloat4().w) + ")";
            case VarientType::Sampler: return "";
        }
        return "";
    }

    static std::vector<std::string> Inner_Attribute_Names = {
        "a_position",
        "a_normal",
        "a_tangent",
        "a_color",
        "a_texcoord0",
        "a_texcoord1",
        "v_worldPosition",
        "v_worldNormal",
        "v_worldTangent",
        "v_color",
        "v_texcoord0",
        "v_texcoord1",
        "v_fragColor"
    };

    Functions& FunctionsImpGL::ConstructVector(const FunctionVarient& inValueX, const FunctionVarient& inValueY, const FunctionVarient& inValueZ, const FunctionVarient& inValueW, const FunctionVarient& outValue)
    {
        m_functions   += outValue.Name + " = vec4(" + "vec4(" + inValueX.Name + ").x" + "vec4(" + inValueY.Name + ").x" + "vec4(" + inValueZ.Name + ").x" + "vec4(" + inValueW.Name + ").x" + ");\n";
        insertValue(inValueX);
        insertValue(inValueY);
        insertValue(inValueZ);
        insertValue(inValueW);
        insertValue(outValue);
        return *this;
    }

    Functions& FunctionsImpGL::SplitVector(const FunctionVarient& inValue, const FunctionVarient& outValueX, const FunctionVarient& outValueY, const FunctionVarient& outValueZ, const FunctionVarient& outValueW)
    {
        m_functions += outValueX.Name + " = " + inValue.Name + ".x;";
        m_functions += outValueY.Name + " = " + inValue.Name + ".y;";
        m_functions += outValueZ.Name + " = " + inValue.Name + ".z;";
        m_functions += outValueW.Name + " = " + inValue.Name + ".w;";
        insertValue(inValue);
        insertValue(outValueX);
        insertValue(outValueY);
        insertValue(outValueZ);
        insertValue(outValueW);
        return *this;
    }

    Functions& FunctionsImpGL::Assignment(const FunctionVarient& inValue, const FunctionVarient& outValue)
    {
        m_functions   += outValue.Name + " = " + inValue.Name + ";\n";
        insertValue(inValue);
        insertValue(outValue);
        return *this;
    }

    Functions& FunctionsImpGL::TextureFetch(const FunctionVarient& sampler, const FunctionVarient& texcoord, const FunctionVarient& outValue)
    {
        m_functions += outValue.Name + " = texture(" + sampler.Name + ", " + texcoord.Name + ");\n";
        insertValue(sampler);
        insertValue(texcoord);
        insertValue(outValue);
        return *this;
    }

    Functions& FunctionsImpGL::Abs(const FunctionVarient& inValue, const FunctionVarient& outValue)
    {
        m_functions += outValue.Name + " = abs(" + inValue.Name + ");\n";
        insertValue(inValue);
        insertValue(outValue);
        return *this;
    }

    Functions& FunctionsImpGL::Add(const FunctionVarient& inA, const FunctionVarient& inB, const FunctionVarient& outValue)
    {
        m_functions   += outValue.Name + " = " + inA.Name + " + " + inB.Name + ";\n";
        insertValue(inA);
        insertValue(inB);
        insertValue(outValue);
        return *this;
    }

    Functions& FunctionsImpGL::Ceil(const FunctionVarient& inValue, const FunctionVarient& outValue)
    {
        m_functions   += outValue.Name + " = ceil(" + inValue.Name + ");\n";
        insertValue(inValue);
        insertValue(outValue);
        return *this;
    }

    Functions& FunctionsImpGL::Clamp(const FunctionVarient& inA, const FunctionVarient& inMin, const FunctionVarient& inMax, const FunctionVarient& outValue)
    {
        m_functions += outValue.Name + " = clamp(" + inA.Name + ", " + inMin.Name + ", " + inMax.Name + ");\n";
        insertValue(inA);
        insertValue(inMin);
        insertValue(inMax);
        insertValue(outValue);
        return *this;
    }

    Functions& FunctionsImpGL::Divide(const FunctionVarient& inA, const FunctionVarient& inB, const FunctionVarient& outValue)
    {
        m_functions += outValue.Name + " = " + inA.Name + " / " + inB.Name + ";\n";
        insertValue(inA);
        insertValue(inB);
        insertValue(outValue);
        return *this;
    }

    Functions& FunctionsImpGL::DDX(const FunctionVarient& inValue, const FunctionVarient& outValue)
    {
        m_functions = outValue.Name + " = dFdx(" + inValue.Name + ");\n";
        insertValue(inValue);
        insertValue(outValue);
        return *this;
    }

    Functions& FunctionsImpGL::DDY(const FunctionVarient& inValue, const FunctionVarient& outValue)
    {
        m_functions = outValue.Name + " = dFdy(" + inValue.Name + ");\n";
        insertValue(inValue);
        insertValue(outValue);
        return *this;
    }

    Functions& FunctionsImpGL::Discard(const FunctionVarient& inA, const FunctionVarient& inBool, const FunctionVarient& outValue)
    {
        m_functions += "if (" + inBool.Name + "){ discard; }\n" + outValue.Name + " = " + inA.Name + ";\n";
        insertValue(inA);
        insertValue(inBool);
        insertValue(outValue);
        return *this;
    }

    Functions& FunctionsImpGL::Floor(const FunctionVarient& inA, const FunctionVarient& outValue)
    {
        m_functions   += outValue.Name + " = floor(" + inA.Name + ");\n";
        insertValue(inA);
        insertValue(outValue);
        return *this;
    }

    Functions& FunctionsImpGL::Fract(const FunctionVarient& inA, const FunctionVarient& outValue)
    {
        m_functions   += outValue.Name + " = fract(" + inA.Name + ");\n";
        insertValue(inA);
        insertValue(outValue);
        return *this;
    }

    Functions& FunctionsImpGL::Max(const FunctionVarient& inA, const FunctionVarient& inB, const FunctionVarient& outValue)
    {
        m_functions   += outValue.Name + " = max(" + inA.Name + ", " + inB.Name + ");\n";
        insertValue(inA);
        insertValue(inB);
        insertValue(outValue);
        return *this;
    }

    Functions& FunctionsImpGL::Min(const FunctionVarient& inA, const FunctionVarient& inB, const FunctionVarient& outValue)
    {
        m_functions   += outValue.Name + " = min(" + inA.Name + ", " + inB.Name + ");\n";
        insertValue(inA);
        insertValue(inB);
        insertValue(outValue);
        return *this;
    }

    Functions& FunctionsImpGL::Mix(const FunctionVarient& inA, const FunctionVarient& inB, const FunctionVarient& inAlpha, const FunctionVarient& outValue)
    {
        m_functions   += outValue.Name + " = mix(" + inA.Name + ", " + inB.Name + ", " + inAlpha.Name + ");\n";
        insertValue(inA);
        insertValue(inB);
        insertValue(inAlpha);
        insertValue(outValue);
        return *this;
    }

    Functions& FunctionsImpGL::Mod(const FunctionVarient& inA, const FunctionVarient& inDivider, const FunctionVarient& outValue)
    {
        m_functions   += outValue.Name + " = mod(" + inA.Name + ", " + inDivider.Name + ");\n";
        insertValue(inA);
        insertValue(inDivider);
        insertValue(outValue);
        return *this;
    }

    Functions& FunctionsImpGL::Multiply(const FunctionVarient& inA, const FunctionVarient& inB, const FunctionVarient& outValue)
    {
        m_functions   += outValue.Name + " = " + inA.Name + " * " + inB.Name + ";\n";
        insertValue(inA);
        insertValue(inB);
        insertValue(outValue);
        return *this;
    }

    Functions& FunctionsImpGL::Negate(const FunctionVarient& inA, const FunctionVarient& outValue)
    {
        m_functions   += outValue.Name + " = -" + inA.Name + ";\n";
        insertValue(inA);
        insertValue(outValue);
        return *this;
    }

    Functions& FunctionsImpGL::Round(const FunctionVarient& inA, const FunctionVarient& outValue)
    {
        m_functions   += outValue.Name + " = floor(" + inA.Name + " + 0.5);\n";
        insertValue(inA);
        insertValue(outValue);
        return *this;
    }

    Functions& FunctionsImpGL::Sign(const FunctionVarient& inA, const FunctionVarient& outValue)
    {
        m_functions   += outValue.Name + " = sign(" + inA.Name + ");\n";
        insertValue(inA);
        insertValue(outValue);
        return *this;
    }

    Functions& FunctionsImpGL::SmoothStep(const FunctionVarient& inA, const FunctionVarient& inB, const FunctionVarient& inAlpha, const FunctionVarient& outValue)
    {
        m_functions   += outValue.Name + " = smoothstep(" + inA.Name + ", " + inB.Name + ", " + inAlpha.Name + ");\n";
        insertValue(inA);
        insertValue(inB);
        insertValue(inAlpha);
        insertValue(outValue);
        return *this;
    }

    Functions& FunctionsImpGL::Step(const FunctionVarient& inA, const FunctionVarient& inB, const FunctionVarient& outValue)
    {
        m_functions   += outValue.Name + " = step(" + inA.Name + ", " + inB.Name + ");\n";
        insertValue(inA);
        insertValue(inB);
        insertValue(outValue);
        return *this;
    }

    Functions& FunctionsImpGL::Subtract(const FunctionVarient& inA, const FunctionVarient& inB, const FunctionVarient& outValue)
    {
        m_functions   += outValue.Name + " = " + inA.Name + " - " + inB.Name + ";\n";
        insertValue(inA);
        insertValue(inB);
        insertValue(outValue);
        return *this;
    }

    void FunctionsImpGL::insertValue(const FunctionVarient& value)
    {
        auto vIter = std::find_if(m_varients.begin(), m_varients.end(), [&](const FunctionVarient& var){
            return value.Name == var.Name;
        });
        if (vIter == m_varients.end()) {
            m_varients.push_back(value);
        }
    }

    void MaterialVM::SetUniform(const FunctionVarient& var)
    {
        m_uniforms.push_back(var);
    }

    MaterialGLVM::MaterialGLVM(const std::shared_ptr<Lutra::Material>& material)
    : m_material(material)
    {
        m_vertexFuns.reset(new FunctionsImpGL);
        m_fragmentFuns.reset(new FunctionsImpGL);
    }

    bool MaterialGLVM::CheckVarientAvailable(const FunctionVarient& var)
    {
        auto uIter = std::find_if(m_uniforms.begin(), m_uniforms.end(), [&](const FunctionVarient& varient){
            return varient.Name == var.Name;
        });
        auto aIter = std::find(Inner_Attribute_Names.begin(), Inner_Attribute_Names.end(), var.Name);
        return uIter == m_uniforms.end() && aIter == Inner_Attribute_Names.end();
    }

    void MaterialGLVM::Compile()
    {
        std::string Header;
        
        for (auto& var : m_uniforms) {
            Header += "uniform " + GetValueType(var.Value.GetType()) + " " + var.Name + ";\n";
        }
        
        std::string VertexBody;
        {
            auto vertexFuns = static_cast<FunctionsImpGL*>(m_vertexFuns.get());
            VertexBody += "in vec3 a_position;\n";
            VertexBody += "in vec3 a_normal;\n";
            VertexBody += "in vec3 a_tangent;\n";
            VertexBody += "in vec4 a_color;\n";
            VertexBody += "in vec2 a_texcoord0;\n";
            VertexBody += "in vec2 a_texcoord1;\n";
            VertexBody += "out vec3 v_worldPosition;\n";
            VertexBody += "out vec3 v_worldNormal;\n";
            VertexBody += "out vec3 v_worldTangent;\n";
            VertexBody += "out vec4 v_color;\n";
            VertexBody += "out vec2 v_texcoord0;\n";
            VertexBody += "out vec2 v_texcoord1;\n";
            VertexBody += "uniform mat4 u_ViewProjMat;\n";
            VertexBody += "uniform mat4 u_ModelMat;\n";
            VertexBody += "uniform mat3 u_NormalWorldMat;\n";
            VertexBody += Header;
            VertexBody += "void main()";
            VertexBody += "{\n";
            VertexBody += "v_worldPosition = (u_ModelMat * vec4(a_position, 1.0)).xyz;\n";
            VertexBody += "v_worldNormal = u_NormalWorldMat * a_normal;\n";
            VertexBody += "v_worldTangent = u_NormalWorldMat * a_tangent;\n";
            VertexBody += "v_color = a_color;\n";
            VertexBody += "v_texcoord0 = a_texcoord0;\n";
            VertexBody += "v_texcoord1 = a_texcoord1;\n";
            for (auto& var : vertexFuns->m_varients) {
                if (CheckVarientAvailable(var))
                    VertexBody += GetValueType(var.Value.GetType()) + " " + var.Name + " = " + GetValue(var.Value) + ";\n";
            }
            VertexBody += vertexFuns->m_functions;
            VertexBody += "gl_Position = u_ViewProjMat * vec4(v_worldPosition, 1.0);\n";
            VertexBody += "}\n";
            vertexFuns->m_varients.clear();
            vertexFuns->m_functions.clear();
        }
        
        std::string FragmentBody;
        {
            auto fragmentFuns = static_cast<FunctionsImpGL*>(m_fragmentFuns.get());
            FragmentBody += "in vec3 v_worldPosition;\n";
            FragmentBody += "in vec3 v_worldNormal;\n";
            FragmentBody += "in vec3 v_worldTangent;\n";
            FragmentBody += "in vec4 v_color;\n";
            FragmentBody += "in vec2 v_texcoord0;\n";
            FragmentBody += "in vec2 v_texcoord1;\n";
            FragmentBody += "out vec4 v_fragColor;\n";
            FragmentBody += Header;
            FragmentBody += "void main()";
            FragmentBody += "{\n";
            for (auto& var : fragmentFuns->m_varients) {
                if (CheckVarientAvailable(var))
                    FragmentBody += GetValueType(var.Value.GetType()) + " " + var.Name + " = " + GetValue(var.Value) + ";\n";
            }
            FragmentBody += fragmentFuns->m_functions;
            FragmentBody += "}\n";
            fragmentFuns->m_varients.clear();
            fragmentFuns->m_functions.clear();
        }
        //printf("Vertex - %s\n\n\nFragment-%s", VertexBody.c_str(), FragmentBody.c_str());
        auto pass = std::shared_ptr<Lutra::Pass>(new Lutra::Pass({
            VertexBody,
            FragmentBody,
            m_material->GetPass(0)->GetBlendMode(),
            m_material->GetPass(0)->GetCullMode(),
            m_material->GetPass(0)->GetDepthTest(),
            m_material->GetPass(0)->GetDepthWrite(),
            m_material->GetPass(0)->GetTwoSided(),
            m_material->GetPass(0)->GetColorMask()
        }));
        
        for (auto& var : m_uniforms) {
            switch (var.Value.GetType()) {
                case VarientType::Bool:
                    pass->GetShader().SetBool(var.Name, var.Value.AsBool());
                    break;
                case VarientType::Int:
                    pass->GetShader().SetInt(var.Name, var.Value.AsInt());
                    break;
                case VarientType::Float:
                    pass->GetShader().SetFloat(var.Name, var.Value.AsFloat());
                    break;
                case VarientType::Float2:
                    pass->GetShader().SetFloat2(var.Name, var.Value.AsFloat2());
                    break;
                case VarientType::Float3:
                    pass->GetShader().SetFloat3(var.Name, var.Value.AsFloat3());
                    break;
                case VarientType::Float4:
                    pass->GetShader().SetFloat4(var.Name, var.Value.AsFloat4());
                    break;
                case VarientType::Sampler:
                    pass->GetShader().SetSampler(var.Name, var.Value.AsSampler());
                    break;
                    
                default:
                    break;
            }
        }
        m_material->SetPass(0, pass);
        
        m_uniforms.clear();
    }
}
