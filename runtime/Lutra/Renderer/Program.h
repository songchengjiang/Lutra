//
//  Program.hpp
//  Lutra
//
//  Created by JasonCheng on 2021/1/6.
//

#ifndef Program_hpp
#define Program_hpp
#include <string>
#include <glm.hpp>
#include "DeviceResource.h"

namespace Lutra {

    class DeviceTexture;
    class Program : public DeviceResource
    {
    public:
        
        ~Program() = default;
        
        virtual void Bind() = 0;
        virtual void Unbind() = 0;

        virtual void SetUniform(const std::string& name, const int value[], int count) = 0;
        virtual void SetUniform(const std::string& name, const float value[], int count) = 0;
        virtual void SetUniform(const std::string& name, const glm::vec2 value[], int count) = 0;
        virtual void SetUniform(const std::string& name, const glm::vec3 value[], int count) = 0;
        virtual void SetUniform(const std::string& name, const glm::vec4 value[], int count) = 0;
        virtual void SetUniform(const std::string& name, const glm::mat3 value[], int count) = 0;
        virtual void SetUniform(const std::string& name, const glm::mat4 value[], int count) = 0;
        
        virtual bool IsUniformActived(const std::string& name) = 0;
        
        virtual void SetSampler(const std::string& name, const std::shared_ptr<DeviceTexture>& tex, uint32_t slot) = 0;
    };

}

#endif /* Program_hpp */
