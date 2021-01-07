//
//  Program_GL.hpp
//  Lutra
//
//  Created by JasonCheng on 2021/1/6.
//

#ifndef Program_GL_hpp
#define Program_GL_hpp

#include "Program.h"
#include "GLHeader.h"

namespace Lutra {

    class ProgramGL : public Program
    {
    public:
        
        ProgramGL(const std::string& vShader, const std::string& fShader);
        ~ProgramGL() = default;
        
        virtual void Bind() const override;
        virtual void Unbind() const override;
        
        virtual void SetUniform(const std::string& name, const float value[], int count) override;
        virtual void SetUniform(const std::string& name, const glm::vec2 value[], int count) override;
        virtual void SetUniform(const std::string& name, const glm::vec3 value[], int count) override;
        virtual void SetUniform(const std::string& name, const glm::vec4 value[], int count) override;
        virtual void SetUniform(const std::string& name, const glm::mat3 value[], int count) override;
        virtual void SetUniform(const std::string& name, const glm::mat4 value[], int count) override;
        
        virtual void SetSampler(const std::string& name, const std::shared_ptr<DeviceTexture>& tex, uint32_t slot) override;
        
    private:
        
        GLuint m_id;
    };

}

#endif /* Program_GL_hpp */
