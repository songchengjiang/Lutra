//
//  Shader.cpp
//  Lutra
//
//  Created by JasonCheng on 2021/1/5.
//

#include "Shader.h"

namespace Lutra {

    Shader::Shader(const std::string& vertex, const std::string& fragment)
        : m_vertexSource(vertex)
        , m_fragmentSource(fragment)
    {
        
    }

    Shader::~Shader()
    {
        for (auto& value : m_valueMap) {
            switch (value.second.Type_) {
                case ShaderValue::Type::Sampler:
                    delete value.second.Value_.tex;
                    break;
                case ShaderValue::Type::Mat3:
                    delete value.second.Value_.m3;
                    break;
                case ShaderValue::Type::Mat4:
                    delete value.second.Value_.m4;
                    break;
                    
                default:
                    break;
            }
        }
    }

    void Shader::SetFloat(const std::string& name, float value)
    {
        m_valueMap[name].Type_ = ShaderValue::Type::Float;
        m_valueMap[name].Value_.v1 = value;
    }

    void Shader::SetFloat2(const std::string& name, const glm::vec2& value)
    {
        m_valueMap[name].Type_ = ShaderValue::Type::Float2;
        m_valueMap[name].Value_.v2 = value;
    }

    void Shader::SetFloat3(const std::string& name, const glm::vec3& value)
    {
        m_valueMap[name].Type_ = ShaderValue::Type::Float3;
        m_valueMap[name].Value_.v3 = value;
    }

    void Shader::SetFloat4(const std::string& name, const glm::vec4& value)
    {
        m_valueMap[name].Type_ = ShaderValue::Type::Float4;
        m_valueMap[name].Value_.v4 = value;
    }
    
    void Shader::SetSampler(const std::string& name, const std::shared_ptr<Texture>& value)
    {
        m_valueMap[name].Type_ = ShaderValue::Type::Sampler;
        m_valueMap[name].Value_.tex = new std::shared_ptr<Texture>(value);
    }

    void Shader::SetMat3(const std::string& name, const glm::mat3& value)
    {
        m_valueMap[name].Type_ = ShaderValue::Type::Mat4;
        m_valueMap[name].Value_.m3 = new glm::mat3(value);
    }

    void Shader::SetMat4(const std::string& name, const glm::mat4& value)
    {
        m_valueMap[name].Type_ = ShaderValue::Type::Mat4;
        m_valueMap[name].Value_.m4 = new glm::mat4(value);
    }

}
