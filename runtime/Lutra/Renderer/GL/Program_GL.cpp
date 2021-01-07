//
//  Program_GL.cpp
//  Lutra
//
//  Created by JasonCheng on 2021/1/6.
//

#include "Program_GL.h"
#include "LutraAssert.h"
#include "DeviceTexture_GL.h"
#include <array>
#include <vector>
#include <gtc/type_ptr.hpp>

namespace Lutra {

    ProgramGL::ProgramGL(const std::string& vShader, const std::string& fShader)
    {
        GLuint program = glCreateProgram();
        GLuint glShaderIDs[2];
        uint32_t shaderIndex = 0;
        auto shaderCompile = [&](GLenum shaderType, const std::string& source){
            GLuint shader = glCreateShader(shaderType);
            const GLchar* sourceCStr = source.c_str();
            glShaderSource(shader, 1, &sourceCStr, 0);

            glCompileShader(shader);

            GLint isCompiled = 0;
            glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled);
            if (isCompiled == GL_FALSE)
            {
                GLint maxLength = 0;
                glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);

                std::vector<GLchar> infoLog(maxLength);
                glGetShaderInfoLog(shader, maxLength, &maxLength, &infoLog[0]);

                glDeleteShader(shader);

                LT_CORE_ASSERT(false, "Shader compilation failure!");
            }

            glAttachShader(program, shader);
            glShaderIDs[shaderIndex++] = shader;
        };
        
        shaderCompile(GL_VERTEX_SHADER, vShader);
        shaderCompile(GL_FRAGMENT_SHADER, fShader);
        
        m_id = program;

        // Link our program
        glLinkProgram(program);

        // Note the different functions here: glGetProgram* instead of glGetShader*.
        GLint isLinked = 0;
        glGetProgramiv(program, GL_LINK_STATUS, (int*)&isLinked);
        if (isLinked == GL_FALSE)
        {
            GLint maxLength = 0;
            glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

            // The maxLength includes the NULL character
            std::vector<GLchar> infoLog(maxLength);
            glGetProgramInfoLog(program, maxLength, &maxLength, &infoLog[0]);

            // We don't need the program anymore.
            glDeleteProgram(program);
            
            for (auto id : glShaderIDs)
                glDeleteShader(id);

            LT_CORE_ASSERT(false, "Shader link failure!");
            return;
        }

        for (auto id : glShaderIDs)
        {
            glDetachShader(program, id);
            glDeleteShader(id);
        }
    }

    void ProgramGL::Bind() const
    {
        glUseProgram(m_id);
    }

    void ProgramGL::Unbind() const
    {
        glUseProgram(0);
    }
        
    void ProgramGL::SetUniform(const std::string& name, const float value[], int count)
    {
        GLint location = glGetUniformLocation(m_id, name.c_str());
        glUniform1fv(location, count, value);
    }

    void ProgramGL::SetUniform(const std::string& name, const glm::vec2 value[], int count)
    {
        GLint location = glGetUniformLocation(m_id, name.c_str());
        glUniform2fv(location, count, glm::value_ptr(value[0]));
    }

    void ProgramGL::SetUniform(const std::string& name, const glm::vec3 value[], int count)
    {
        GLint location = glGetUniformLocation(m_id, name.c_str());
        glUniform3fv(location, count, glm::value_ptr(value[0]));
    }

    void ProgramGL::SetUniform(const std::string& name, const glm::vec4 value[], int count)
    {
        GLint location = glGetUniformLocation(m_id, name.c_str());
        glUniform4fv(location, count, glm::value_ptr(value[0]));
    }

    void ProgramGL::SetUniform(const std::string& name, const glm::mat3 value[], int count)
    {
        GLint location = glGetUniformLocation(m_id, name.c_str());
        glUniformMatrix3fv(location, count, GL_FALSE, glm::value_ptr(value[0]));
    }

    void ProgramGL::SetUniform(const std::string& name, const glm::mat4 value[], int count)
    {
        GLint location = glGetUniformLocation(m_id, name.c_str());
        glUniformMatrix4fv(location, count, GL_FALSE, glm::value_ptr(value[0]));
    }

    void ProgramGL::SetSampler(const std::string& name, const std::shared_ptr<DeviceTexture>& tex, uint32_t slot)
    {
        GLint location = glGetUniformLocation(m_id, name.c_str());
        glUniform1i(location, slot);
        glActiveTexture(GL_TEXTURE0 + slot);
        tex->Bind();
    }

}
