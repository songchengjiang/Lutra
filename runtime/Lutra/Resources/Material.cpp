//
//  Material.cpp
//  Lutra
//
//  Created by JasonCheng on 2021/1/4.
//

#include "Material.h"
#include "Pass.h"
#include "Stream.h"
#include "Texture.h"
#include "ResourceManager.h"

namespace Lutra {

    Material::Material()
    {
    }

    void Material::Serialize(WriteStream& stream)
    {
        Resource::Serialize(stream);
        stream.BeginArray("Passes");
        for (auto& pass : m_passList) {
            stream.BeginMap(std::string());
            stream.WriteValue("BlendMode", (int)pass->GetBlendMode());
            stream.WriteValue("CullMode", (int)pass->GetCullMode());
            stream.WriteValue("DepthWrite", pass->GetDepthWrite());
            stream.WriteValue("DepthTest", pass->GetDepthTest());
            stream.WriteValue("TwoSided", pass->GetTwoSided());
            stream.WriteValue("ColorMask", pass->GetColorMask());
                stream.BeginMap("Shader");
                stream.BeginArray("Uniforms");
                for (auto &value : pass->GetShader().GetValues()) {
                    stream.BeginMap("");
                    stream.WriteValue("Type", (int)value.second.Type_);
                    stream.WriteValue("Name", value.first);
                    switch (value.second.Type_) {
                        case ShaderValue::Type::Bool: stream.WriteValue("Value", value.second.Value_.b); break;
                        case ShaderValue::Type::Int: stream.WriteValue("Value", value.second.Value_.i); break;
                        case ShaderValue::Type::Float: stream.WriteValue("Value", value.second.Value_.v1); break;
                        case ShaderValue::Type::Float2: stream.WriteValue("Value", value.second.Value_.v2); break;
                        case ShaderValue::Type::Float3: stream.WriteValue("Value", value.second.Value_.v3); break;
                        case ShaderValue::Type::Float4: stream.WriteValue("Value", value.second.Value_.v4); break;
                        case ShaderValue::Type::Sampler: stream.WriteValue("Value", (*value.second.Value_.tex) != nullptr? (*value.second.Value_.tex)->GetUUID().str(): ""); break;
                        case ShaderValue::Type::Mat3: stream.WriteValue("Value", *value.second.Value_.m3); break;
                        case ShaderValue::Type::Mat4: stream.WriteValue("Value", *value.second.Value_.m4); break;
                    }
                    stream.EndMap();
                }
                stream.EndArray();
                stream.WriteValue("VertexShader", pass->GetShader().GetVertexSource());
                stream.WriteValue("FragmentShader", pass->GetShader().GetFragmentSource());
                stream.EndMap();
            stream.EndMap();
        }
        stream.EndArray();
    }

    void Material::Deserialize(ReadStream& stream)
    {
        Resource::Deserialize(stream);
        size_t size = stream.BeginArray("Passes");
        PassBuilder builder;
        for (size_t i = 0; i < size; ++i) {
            stream.EnterArray(i);
            int mode = 0;
            stream.ReadValue("BlendMode", mode); builder.BdMode = (BlendMode)mode;
            stream.ReadValue("CullMode", mode); builder.ClMode = (CullMode)mode;
            stream.ReadValue("DepthWrite", builder.DepthWrite);
            stream.ReadValue("DepthTest", builder.DepthTest);
            stream.ReadValue("TwoSided", builder.TwoSided);
            stream.ReadValue("ColorMask", builder.ColorMask);
                stream.BeginMap("Shader");
                stream.ReadValue("VertexShader", builder.VertexShader);
                stream.ReadValue("FragmentShader", builder.FragmentShader);
                auto pass = std::shared_ptr<Pass>(new Pass(builder));
                size_t uSize = stream.BeginArray("Uniforms");
                for (size_t k = 0; k < uSize; ++k) {
                    stream.EnterArray(k);
                    int type;
                    stream.ReadValue("Type", type);
                    std::string name;
                    stream.ReadValue("Name", name);
                    switch ((ShaderValue::Type)type) {
                        case ShaderValue::Type::Bool:
                        {
                            bool value;
                            stream.ReadValue("Value", value);
                            pass->GetShader().SetInt(name, value);
                        }
                            break;
                        case ShaderValue::Type::Int:
                        {
                            int value;
                            stream.ReadValue("Value", value);
                            pass->GetShader().SetInt(name, value);
                        }
                            break;
                        case ShaderValue::Type::Float:
                        {
                            float value;
                            stream.ReadValue("Value", value);
                            pass->GetShader().SetFloat(name, value);
                        }
                            break;
                        case ShaderValue::Type::Float2:
                        {
                            glm::vec2 value;
                            stream.ReadValue("Value", value);
                            pass->GetShader().SetFloat2(name, value);
                        }
                            break;
                        case ShaderValue::Type::Float3:
                        {
                            glm::vec3 value;
                            stream.ReadValue("Value", value);
                            pass->GetShader().SetFloat3(name, value);
                        }
                            break;
                        case ShaderValue::Type::Float4:
                        {
                            glm::vec4 value;
                            stream.ReadValue("Value", value);
                            pass->GetShader().SetFloat4(name, value);
                        }
                            break;
                        case ShaderValue::Type::Sampler:
                        {
                            std::string uuid;
                            stream.ReadValue("Value", uuid);
                            pass->GetShader().SetSampler(name, ResourceManager::Instance().LoadResource<Texture>(sole::rebuild(uuid)));
                        }
                            break;
                        case ShaderValue::Type::Mat3:
                        {
                            glm::mat3 value;
                            stream.ReadValue("Value", value);
                            pass->GetShader().SetMat3(name, value);
                        }
                            break;
                        case ShaderValue::Type::Mat4:
                        {
                            glm::mat4 value;
                            stream.ReadValue("Value", value);
                            pass->GetShader().SetMat4(name, value);
                        }
                            break;
                    }
                    stream.LeaveArray();
                }
                stream.EndArray();
                stream.EndMap();
            stream.LeaveArray();
            AddPass(pass);
        }
        
        
        stream.EndArray();
    }

    void Material::AddPass(const std::shared_ptr<Pass>& pass)
    {
        m_passList.push_back(pass);
    }

    void Material::SetPass(size_t idx, const std::shared_ptr<Pass>& pass)
    {
        m_passList[idx] = pass;
    }

}
