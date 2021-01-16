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
        
        GLint major = 0;
        GLint minor = 0;
        glGetIntegerv(GL_MAJOR_VERSION, &major);
        glGetIntegerv(GL_MINOR_VERSION, &minor);
        std::string header = "#version " + std::to_string(major) + std::to_string(minor) + "0\n";
        auto shaderCompile = [&](GLenum shaderType, const std::string& source){
            GLuint shader = glCreateShader(shaderType);
            std::string finalSource = header + source;
            const GLchar* sourceCStr = finalSource.c_str();
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
        
        GLint activeAttribs  = 0;
        GLint activeUniforms = 0;
        GLint max0, max1;
        glGetProgramiv(m_id, GL_ACTIVE_ATTRIBUTE_MAX_LENGTH, &max0);
        glGetProgramiv(m_id, GL_ACTIVE_UNIFORM_MAX_LENGTH,   &max1);
        glGetProgramiv(m_id, GL_ACTIVE_ATTRIBUTES, &activeAttribs);
        glGetProgramiv(m_id, GL_ACTIVE_UNIFORMS,   &activeUniforms);
        
        uint32_t maxLength = std::max(max0, max1);
        char* name = new char[maxLength + 1];
        
        for (uint32_t i = 0; i < activeAttribs; ++i) {
            GLint size;
            GLenum type = 0;
            glGetActiveAttrib(m_id, i, maxLength + 1, NULL, &size, &type, name);
            GLint loc = glGetAttribLocation(m_id, name);
            m_attributeMap[name] = loc;
        }
        
        for (uint32_t i = 0; i < activeUniforms; ++i) {
            GLenum gltype;
            GLint num;
            glGetActiveUniform(m_id, i, maxLength + 1, NULL, &num, &gltype, name);
            GLint loc = glGetUniformLocation(m_id, name);
            m_uniformMap[name] = loc;
        }
        
        
        delete [] name;
    }

    void ProgramGL::Bind()
    {
        glUseProgram(m_id);
        UpdateUsage();
    }

    void ProgramGL::Unbind()
    {
        glUseProgram(0);
    }
        
    void ProgramGL::SetUniform(const std::string& name, const float value[], int count)
    {
        auto iter = m_uniformMap.find(name);
        if (iter != m_uniformMap.end())
            glUniform1fv(iter->second, count, value);
    }

    void ProgramGL::SetUniform(const std::string& name, const glm::vec2 value[], int count)
    {
        auto iter = m_uniformMap.find(name);
        if (iter != m_uniformMap.end())
            glUniform2fv(iter->second, count, glm::value_ptr(value[0]));
    }

    void ProgramGL::SetUniform(const std::string& name, const glm::vec3 value[], int count)
    {
        auto iter = m_uniformMap.find(name);
        if (iter != m_uniformMap.end())
            glUniform3fv(iter->second, count, glm::value_ptr(value[0]));
    }

    void ProgramGL::SetUniform(const std::string& name, const glm::vec4 value[], int count)
    {
        auto iter = m_uniformMap.find(name);
        if (iter != m_uniformMap.end())
            glUniform4fv(iter->second, count, glm::value_ptr(value[0]));
    }

    void ProgramGL::SetUniform(const std::string& name, const glm::mat3 value[], int count)
    {
        auto iter = m_uniformMap.find(name);
        if (iter != m_uniformMap.end())
            glUniformMatrix3fv(iter->second, count, GL_FALSE, glm::value_ptr(value[0]));
    }

    void ProgramGL::SetUniform(const std::string& name, const glm::mat4 value[], int count)
    {
        auto iter = m_uniformMap.find(name);
        if (iter != m_uniformMap.end())
            glUniformMatrix4fv(iter->second, count, GL_FALSE, glm::value_ptr(value[0]));
    }

    void ProgramGL::SetSampler(const std::string& name, const std::shared_ptr<DeviceTexture>& tex, uint32_t slot)
    {   auto iter = m_uniformMap.find(name);
        if (iter != m_uniformMap.end()) {
            glUniform1i(iter->second, slot);
            glActiveTexture(GL_TEXTURE0 + slot);
            tex->Bind();
        }
    }

}
