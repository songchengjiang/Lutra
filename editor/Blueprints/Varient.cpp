//
//  Varient.cpp
//  Editor
//
//  Created by JasonCheng on 2021/1/19.
//

#include "Varient.h"

namespace LutraEditor {

    Varient::Varient(bool value)
    {
        m_type = VarientType::Bool;
        m_value.b = value;
    }

    Varient::Varient(int value)
    {
        m_type = VarientType::Int;
        m_value.i = value;
    }

    Varient::Varient(float value)
    {
        m_type = VarientType::Float;
        m_value.f = value;
    }

    Varient::Varient(const glm::vec2& value)
    {
        m_type = VarientType::Float2;
        m_value.f2 = value;
    }

    Varient::Varient(const glm::vec3& value)
    {
        m_type = VarientType::Float3;
        m_value.f3 = value;
    }

    Varient::Varient(const glm::vec4& value)
    {
        m_type = VarientType::Float4;
        m_value.f4 = value;
    }

    Varient::Varient(const std::shared_ptr<Lutra::Texture>& value)
    {
        m_type = VarientType::Sampler;
        m_value.t = new std::shared_ptr<Lutra::Texture>(value);
    }

    Varient::Varient(const Varient& copy)
    : m_type(copy.m_type)
    {
        switch (m_type) {
            case VarientType::Bool:
                m_value.b = copy.m_value.b;
                break;
            case VarientType::Int:
                m_value.i = copy.m_value.i;
                break;
            case VarientType::Float:
                m_value.f = copy.m_value.f;
                break;
            case VarientType::Float2:
                m_value.f2 = copy.m_value.f2;
                break;
            case VarientType::Float3:
                m_value.f3 = copy.m_value.f3;
                break;
            case VarientType::Float4:
                m_value.f4 = copy.m_value.f4;
                break;
            case VarientType::Sampler:
            {
                m_value.t = new std::shared_ptr<Lutra::Texture>(*copy.m_value.t);
            }
                break;
            default:
                break;
        }
    }

    Varient::~Varient()
    {
        if (m_type == VarientType::Sampler) {
            delete m_value.t;
        }
    }

    Varient& Varient::operator=(const Varient& var)
    {
        m_type = var.m_type;
        switch (m_type) {
            case VarientType::Bool:
                m_value.b = var.m_value.b;
                break;
            case VarientType::Int:
                m_value.i = var.m_value.i;
                break;
            case VarientType::Float:
                m_value.f = var.m_value.f;
                break;
            case VarientType::Float2:
                m_value.f2 = var.m_value.f2;
                break;
            case VarientType::Float3:
                m_value.f3 = var.m_value.f3;
                break;
            case VarientType::Float4:
                m_value.f4 = var.m_value.f4;
                break;
            case VarientType::Sampler:
            {
                if (m_value.t) {
                    delete m_value.t;
                }
                m_value.t = new std::shared_ptr<Lutra::Texture>(*var.m_value.t);
            }
                break;
            default:
                break;
        }
        
        return *this;
    }

    bool Varient::operator==(const Varient& var) const
    {
        if (m_type != var.m_type)
            return false;
        
        switch (m_type) {
            case VarientType::Bool:
                return m_value.b == var.m_value.b;
                break;
            case VarientType::Int:
                return m_value.i == var.m_value.i;
                break;
            case VarientType::Float:
                return m_value.f == var.m_value.f;
                break;
            case VarientType::Float2:
                return m_value.f2 == var.m_value.f2;
                break;
            case VarientType::Float3:
                return m_value.f3 == var.m_value.f3;
                break;
            case VarientType::Float4:
                return m_value.f4 == var.m_value.f4;
                break;
            case VarientType::Sampler:
                return *m_value.t == *var.m_value.t;
                break;
            default:
                break;
        }
        return false;
    }

    bool Varient::AsBool() const
    {
        return m_type == VarientType::Bool? m_value.b: false;
    }

    int Varient::AsInt() const
    {
        return m_type == VarientType::Int? m_value.i: 0;
    }

    float Varient::AsFloat() const
    {
        return m_type == VarientType::Float? m_value.f: 0.0f;
    }

    glm::vec2 Varient::AsFloat2() const
    {
        return m_type == VarientType::Float2? m_value.f2: glm::vec2(0.0f);
    }

    glm::vec3 Varient::AsFloat3() const
    {
        return m_type == VarientType::Float3? m_value.f3: glm::vec3(0.0f);
    }

    glm::vec4 Varient::AsFloat4() const
    {
        return m_type == VarientType::Float4? m_value.f4: glm::vec4(0.0f);
    }

    std::shared_ptr<Lutra::Texture> Varient::AsSampler() const
    {
        return m_type == VarientType::Sampler? *m_value.t: nullptr;
    }
}
