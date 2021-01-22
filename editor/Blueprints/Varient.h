//
//  Varient.hpp
//  Editor
//
//  Created by JasonCheng on 2021/1/19.
//

#ifndef Varient_hpp
#define Varient_hpp
#include <Lutra.h>

namespace LutraEditor {

    enum class VarientType
    {
        Bool,
        Int,
        Float,
        Float2,
        Float3,
        Float4,
        Sampler,
    };

    class Varient
    {
    public:
        
        Varient() = default;
        Varient(bool value);
        Varient(int value);
        Varient(float value);
        Varient(const glm::vec2& value);
        Varient(const glm::vec3& value);
        Varient(const glm::vec4& value);
        Varient(const std::shared_ptr<Lutra::Texture>& value);
        Varient(const Varient& copy);
        ~Varient();
        
        VarientType GetType() const { return m_type; }
        
        bool AsBool() const;
        int AsInt() const;
        float AsFloat() const;
        glm::vec2 AsFloat2() const;
        glm::vec3 AsFloat3() const;
        glm::vec4 AsFloat4() const;
        std::shared_ptr<Lutra::Texture> AsSampler() const;
        
        Varient& operator=(const Varient& var);
        
        bool operator==(const Varient& var) const;
        
        bool operator!=(const Varient& var) const {
            return !(*this == var);
        }
        
        bool operator<(const Varient& var) const {
            return m_type < var.m_type;
        }
        
    private:
        
        union Value
        {
            bool  b;
            int   i;
            float f;
            glm::vec2 f2;
            glm::vec3 f3;
            glm::vec4 f4;
            std::shared_ptr<Lutra::Texture>* t;
        };
        
        VarientType m_type;
        Value       m_value{0};
    };

}

#endif /* Varient_hpp */
