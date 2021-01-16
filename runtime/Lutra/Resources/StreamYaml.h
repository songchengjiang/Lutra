//
//  FileStreamYaml.hpp
//  Lutra
//
//  Created by JasonCheng on 2021/1/10.
//

#ifndef FileStreamYaml_hpp
#define FileStreamYaml_hpp
#include "Stream.h"
#include "yaml-cpp/yaml.h"
#include <stack>

namespace Lutra {

    class WriteStreamYaml : public WriteStream
    {
    public:
        
        WriteStreamYaml(const std::string& path);
        virtual ~WriteStreamYaml() = default;

        virtual void Open() override;
        virtual void Close() override;
        
        virtual void BeginMap(const std::string& key) override;
        virtual void EndMap() override;
        virtual void BeginArray(const std::string& key) override;
        virtual void EndArray() override;
        virtual void WriteArrayElement(uint32_t value) override;
        virtual void WriteArrayElement(std::string value) override;
        virtual void WriteValue(const std::string& key, bool value) override;
        virtual void WriteValue(const std::string& key, int value) override;
        virtual void WriteValue(const std::string& key, uint32_t value) override;
        virtual void WriteValue(const std::string& key, float value) override;
        virtual void WriteValue(const std::string& key, const glm::vec2& value) override;
        virtual void WriteValue(const std::string& key, const glm::vec3& value) override;
        virtual void WriteValue(const std::string& key, const glm::vec4& value) override;
        virtual void WriteValue(const std::string& key, const glm::bvec4& value) override;
        virtual void WriteValue(const std::string& key, const glm::mat3& value) override;
        virtual void WriteValue(const std::string& key, const glm::mat4& value) override;
        virtual void WriteValue(const std::string& key, const std::string& value) override;
        virtual void WriteValue(const std::string& key, const std::vector<uint16_t>& value) override;
        virtual void WriteValue(const std::string& key, const std::vector<glm::vec2>& value) override;
        virtual void WriteValue(const std::string& key, const std::vector<glm::vec3>& value) override;
        virtual void WriteValue(const std::string& key, const std::vector<glm::vec4>& value) override;
        virtual void WriteValue(const std::string& key, const std::vector<uint8_t>& value) override;
        
    private:
        
        std::string   m_path;
        YAML::Emitter m_emitter;
    };

    class ReadStreamYaml : public ReadStream
    {
    public:
        ReadStreamYaml(const std::string& path);
        virtual ~ReadStreamYaml() = default;
        
        virtual void Open() override;
        virtual void Close() override;
        
        virtual bool HasMap(const std::string& key) override;
        virtual void BeginMap(const std::string& key) override;
        virtual void EndMap() override;
        virtual bool HasArray(const std::string& key) override;
        virtual size_t BeginArray(const std::string& key) override;
        virtual void EndArray() override;
        virtual void EnterArray(size_t i) override;
        virtual void LeaveArray() override;
        virtual void ReadArrayElement(uint32_t& value) override;
        virtual void ReadArrayElement(std::string& value) override;
        virtual bool HasValue(const std::string& key) override;
        virtual void ReadValue(const std::string& key, bool& value) override;
        virtual void ReadValue(const std::string& key, int& value) override;
        virtual void ReadValue(const std::string& key, uint32_t& value) override;
        virtual void ReadValue(const std::string& key, float& value) override;
        virtual void ReadValue(const std::string& key, glm::vec2& value) override;
        virtual void ReadValue(const std::string& key, glm::vec3& value) override;
        virtual void ReadValue(const std::string& key, glm::vec4& value) override;
        virtual void ReadValue(const std::string& key, glm::bvec4& value) override;
        virtual void ReadValue(const std::string& key, glm::mat3& value) override;
        virtual void ReadValue(const std::string& key, glm::mat4& value) override;
        virtual void ReadValue(const std::string& key, std::string& value) override;
        virtual void ReadValue(const std::string& key, std::vector<uint16_t>& value) override;
        virtual void ReadValue(const std::string& key, std::vector<glm::vec2>& value) override;
        virtual void ReadValue(const std::string& key, std::vector<glm::vec3>& value) override;
        virtual void ReadValue(const std::string& key, std::vector<glm::vec4>& value) override;
        virtual void ReadValue(const std::string& key, std::vector<uint8_t>& value) override;
        
    private:
        std::string   m_path;
        YAML::Node    m_data;
        std::stack<YAML::Node> m_nodeStack;
    };
}

#endif /* FileStreamYaml_hpp */
