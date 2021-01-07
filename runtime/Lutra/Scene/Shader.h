//
//  Shader.hpp
//  Lutra
//
//  Created by JasonCheng on 2021/1/5.
//

#ifndef Shader_hpp
#define Shader_hpp
#include <glm.hpp>
#include <string>
#include <memory>
#include <unordered_map>

namespace Lutra {

    class Texture;
    struct ShaderValue
    {
        enum class Type
        {
            Float,
            Float2,
            Float3,
            Float4,
            Sampler,
            Mat3,
            Mat4,
        };
        
        union Value
        {
            float     v1;
            glm::vec2 v2;
            glm::vec3 v3;
            glm::vec4 v4;
            std::shared_ptr<Texture>* tex;
            glm::mat3* m3;
            glm::mat4* m4;
        };
        
        Type Type_;
        Value Value_;
    };

    class Shader
    {
    public:
        Shader(const std::string& vertex, const std::string& fragment);
        ~Shader();

        Shader(const Shader& other) = default;
        
        void SetFloat(const std::string& name, float value);
        void SetFloat2(const std::string& name, const glm::vec2& value);
        void SetFloat3(const std::string& name, const glm::vec3& value);
        void SetFloat4(const std::string& name, const glm::vec4& value);
        void SetSampler(const std::string& name, const std::shared_ptr<Texture>& value);
        void SetMat3(const std::string& name, const glm::mat3& value);
        void SetMat4(const std::string& name, const glm::mat4& value);
        
        float GetFloat(const std::string& name) { return m_valueMap[name].Value_.v1; }
        glm::vec2 GetFloat2(const std::string& name) { return m_valueMap[name].Value_.v2; }
        glm::vec3 GetFloat3(const std::string& name) { return m_valueMap[name].Value_.v3; }
        glm::vec4 GetFloat4(const std::string& name) { return m_valueMap[name].Value_.v4; }
        const std::shared_ptr<Texture>& GetSampler(const std::string& name) { return *m_valueMap[name].Value_.tex; }
        glm::mat3 GetMat3(const std::string& name) { return *m_valueMap[name].Value_.m3; }
        glm::mat4 GetMat4(const std::string& name) { return *m_valueMap[name].Value_.m4; }
        
        const std::string& GetVertexSource() const { return m_vertexSource; }
        const std::string& GetFragmentSource() const { return m_fragmentSource; }
        
        const std::unordered_map<std::string, ShaderValue>& GetValues() const { return m_valueMap; }
        
        Shader& operator=(const Shader& other) = default;
        
    private:
        
        std::unordered_map<std::string, ShaderValue> m_valueMap;
        std::string m_vertexSource;
        std::string m_fragmentSource;
    };
}

#endif /* Shader_hpp */
