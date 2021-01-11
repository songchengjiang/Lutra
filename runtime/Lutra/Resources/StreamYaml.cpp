//
//  FileStreamYaml.cpp
//  Lutra
//
//  Created by JasonCheng on 2021/1/10.
//

#include "StreamYaml.h"
#include <fstream>
#include "base64/base64.h"

namespace YAML {

    template<>
    struct convert<glm::vec2>
    {
        static Node encode(const glm::vec2& rhs)
        {
            Node node;
            node.push_back(rhs.x);
            node.push_back(rhs.y);
            node.SetStyle(EmitterStyle::Flow);
            return node;
        }

        static bool decode(const Node& node, glm::vec2& rhs)
        {
            if (!node.IsSequence() || node.size() != 2)
                return false;

            rhs.x = node[0].as<float>();
            rhs.y = node[1].as<float>();
            return true;
        }
    };

    template<>
    struct convert<glm::vec3>
    {
        static Node encode(const glm::vec3& rhs)
        {
            Node node;
            node.push_back(rhs.x);
            node.push_back(rhs.y);
            node.push_back(rhs.z);
            node.SetStyle(EmitterStyle::Flow);
            return node;
        }

        static bool decode(const Node& node, glm::vec3& rhs)
        {
            if (!node.IsSequence() || node.size() != 3)
                return false;

            rhs.x = node[0].as<float>();
            rhs.y = node[1].as<float>();
            rhs.z = node[2].as<float>();
            return true;
        }
    };

    template<>
    struct convert<glm::vec4>
    {
        static Node encode(const glm::vec4& rhs)
        {
            Node node;
            node.push_back(rhs.x);
            node.push_back(rhs.y);
            node.push_back(rhs.z);
            node.push_back(rhs.w);
            node.SetStyle(EmitterStyle::Flow);
            return node;
        }

        static bool decode(const Node& node, glm::vec4& rhs)
        {
            if (!node.IsSequence() || node.size() != 4)
                return false;

            rhs.x = node[0].as<float>();
            rhs.y = node[1].as<float>();
            rhs.z = node[2].as<float>();
            rhs.w = node[3].as<float>();
            return true;
        }
    };

    template<>
    struct convert<glm::bvec4>
    {
        static Node encode(const glm::bvec4& rhs)
        {
            Node node;
            node.push_back(rhs.x);
            node.push_back(rhs.y);
            node.push_back(rhs.z);
            node.push_back(rhs.w);
            node.SetStyle(EmitterStyle::Flow);
            return node;
        }

        static bool decode(const Node& node, glm::bvec4& rhs)
        {
            if (!node.IsSequence() || node.size() != 4)
                return false;

            rhs.x = node[0].as<bool>();
            rhs.y = node[1].as<bool>();
            rhs.z = node[2].as<bool>();
            rhs.w = node[3].as<bool>();
            return true;
        }
    };

    template<>
    struct convert<glm::mat3>
    {
        static Node encode(const glm::mat3& rhs)
        {
            Node node;
            node.push_back(rhs[0][0]); node.push_back(rhs[0][1]); node.push_back(rhs[0][2]);
            node.push_back(rhs[1][0]); node.push_back(rhs[1][1]); node.push_back(rhs[1][2]);
            node.push_back(rhs[2][0]); node.push_back(rhs[2][1]); node.push_back(rhs[2][2]);
            node.SetStyle(EmitterStyle::Flow);
            return node;
        }

        static bool decode(const Node& node, glm::mat3& rhs)
        {
            if (!node.IsSequence())
                return false;

            rhs[0][0] = node[0].as<float>(); rhs[0][1] = node[1].as<float>(); rhs[0][2] = node[2].as<float>();
            rhs[1][0] = node[3].as<float>(); rhs[1][1] = node[4].as<float>(); rhs[1][2] = node[5].as<float>();
            rhs[2][0] = node[6].as<float>(); rhs[2][1] = node[7].as<float>(); rhs[2][2] = node[8].as<float>();
            return true;
        }
    };

    template<>
    struct convert<glm::mat4>
    {
        static Node encode(const glm::mat4& rhs)
        {
            Node node;
            node.push_back(rhs[0][0]); node.push_back(rhs[0][1]); node.push_back(rhs[0][2]); node.push_back(rhs[0][3]);
            node.push_back(rhs[1][0]); node.push_back(rhs[1][1]); node.push_back(rhs[1][2]); node.push_back(rhs[1][3]);
            node.push_back(rhs[2][0]); node.push_back(rhs[2][1]); node.push_back(rhs[2][2]); node.push_back(rhs[2][3]);
            node.push_back(rhs[3][0]); node.push_back(rhs[3][1]); node.push_back(rhs[3][2]); node.push_back(rhs[3][3]);
            node.SetStyle(EmitterStyle::Flow);
            return node;
        }

        static bool decode(const Node& node, glm::mat4& rhs)
        {
            if (!node.IsSequence())
                return false;

            rhs[0][0] = node[0].as<float>(); rhs[0][1] = node[1].as<float>(); rhs[0][2] = node[2].as<float>(); rhs[0][3] = node[3].as<float>();
            rhs[1][0] = node[4].as<float>(); rhs[1][1] = node[5].as<float>(); rhs[1][2] = node[6].as<float>(); rhs[1][3] = node[7].as<float>();
            rhs[2][0] = node[8].as<float>(); rhs[2][1] = node[9].as<float>(); rhs[2][2] = node[10].as<float>(); rhs[2][3] = node[11].as<float>();
            rhs[3][0] = node[12].as<float>(); rhs[3][1] = node[13].as<float>(); rhs[3][2] = node[14].as<float>(); rhs[3][3] = node[15].as<float>();
            return true;
        }
    };
}

namespace Lutra {

    WriteStreamYaml::WriteStreamYaml(const std::string& path)
    : m_path(path)
    {
    }

    void WriteStreamYaml::Open()
    {
    }

    void WriteStreamYaml::Close()
    {
        std::ofstream fs;
        fs.open(m_path);
        if (fs.is_open()) {
            fs << m_emitter.c_str();
        }
        fs.close();
    }

    void WriteStreamYaml::BeginMap(const std::string& key)
    {
        if (!key.empty())
            m_emitter << YAML::Key << key;
        m_emitter << YAML::BeginMap;
    }

    void WriteStreamYaml::EndMap()
    {
        m_emitter << YAML::EndMap;
    }

    void WriteStreamYaml::BeginArray(const std::string& key)
    {
        m_emitter << YAML::Key << key << YAML::Value << YAML::BeginSeq;
    }

    void WriteStreamYaml::EndArray()
    {
        m_emitter << YAML::EndSeq;
    }

    void WriteStreamYaml::WriteValue(const std::string& key, bool value)
    {
        m_emitter << YAML::Key << key << YAML::Value << value;
    }

    void WriteStreamYaml::WriteValue(const std::string& key, int value)
    {
        m_emitter << YAML::Key << key << YAML::Value << value;
    }

    void WriteStreamYaml::WriteValue(const std::string& key, uint32_t value)
    {
        m_emitter << YAML::Key << key << YAML::Value << value;
    }

    void WriteStreamYaml::WriteValue(const std::string& key, float value)
    {
        m_emitter << YAML::Key << key << YAML::Value << value;
    }

    void WriteStreamYaml::WriteValue(const std::string& key, const glm::vec2& value)
    {
        m_emitter << YAML::Key << key << YAML::Value;
        m_emitter << YAML::Flow;
        m_emitter << YAML::BeginSeq << value.x << value.y << YAML::EndSeq;
    }

    void WriteStreamYaml::WriteValue(const std::string& key, const glm::vec3& value)
    {
        m_emitter << YAML::Key << key << YAML::Value;
        m_emitter << YAML::Flow;
        m_emitter << YAML::BeginSeq << value.x << value.y << value.z << YAML::EndSeq;
    }

    void WriteStreamYaml::WriteValue(const std::string& key, const glm::vec4& value)
    {
        m_emitter << YAML::Key << key << YAML::Value;
        m_emitter << YAML::Flow;
        m_emitter << YAML::BeginSeq << value.x << value.y << value.z << value.w << YAML::EndSeq;
    }

    void WriteStreamYaml::WriteValue(const std::string& key, const glm::bvec4& value)
    {
        m_emitter << YAML::Key << key << YAML::Value;
        m_emitter << YAML::Flow;
        m_emitter << YAML::BeginSeq << value.x << value.y << value.z << value.w << YAML::EndSeq;
    }

    void WriteStreamYaml::WriteValue(const std::string& key, const glm::mat3& value)
    {
        m_emitter << YAML::Key << key << YAML::Value;
        m_emitter << YAML::Flow;
        m_emitter << YAML::BeginSeq <<
        value[0][0] << value[0][1] << value[0][2] <<
        value[1][0] << value[1][1] << value[1][2] <<
        value[2][0] << value[2][1] << value[2][2] <<
        YAML::EndSeq;
    }

    void WriteStreamYaml::WriteValue(const std::string& key, const glm::mat4& value)
    {
        m_emitter << YAML::Key << key << YAML::Value;
        m_emitter << YAML::Flow;
        m_emitter << YAML::BeginSeq <<
        value[0][0] << value[0][1] << value[0][2] << value[0][3] <<
        value[1][0] << value[1][1] << value[1][2] << value[1][3] <<
        value[2][0] << value[2][1] << value[2][2] << value[2][3] <<
        YAML::EndSeq;
    }

    void WriteStreamYaml::WriteValue(const std::string& key, const std::string& value)
    {
        m_emitter << YAML::Key << key << YAML::Value << value;
    }

    void WriteStreamYaml::WriteValue(const std::string& key, const std::vector<uint16_t>& value)
    {
        m_emitter << YAML::Key << key << YAML::Value;
        m_emitter << YAML::Flow;
        m_emitter << YAML::BeginSeq;
        for (auto &v : value) {
            m_emitter << v;
        }
        m_emitter << YAML::EndSeq;
    }

    void WriteStreamYaml::WriteValue(const std::string& key, const std::vector<glm::vec2>& value)
    {
        m_emitter << YAML::Key << key << YAML::Value;
        m_emitter << YAML::Flow;
        m_emitter << YAML::BeginSeq;
        for (auto &v : value) {
            m_emitter << YAML::BeginSeq << v.x << v.y << YAML::EndSeq;
        }
        m_emitter << YAML::EndSeq;
    }

    void WriteStreamYaml::WriteValue(const std::string& key, const std::vector<glm::vec3>& value)
    {
        m_emitter << YAML::Key << key << YAML::Value;
        m_emitter << YAML::Flow;
        m_emitter << YAML::BeginSeq;
        for (auto &v : value) {
            m_emitter << YAML::BeginSeq << v.x << v.y << v.z << YAML::EndSeq;
        }
        m_emitter << YAML::EndSeq;
    }

    void WriteStreamYaml::WriteValue(const std::string& key, const std::vector<uint8_t>& value)
    {
        std::string encode = base64_encode({value.begin(), value.end()});
        m_emitter << YAML::Key << key << YAML::Value << encode;
    }

    ReadStreamYaml::ReadStreamYaml(const std::string& path)
    : m_path(path)
    {
        
    }

    void ReadStreamYaml::Open()
    {
        m_data = YAML::LoadFile(m_path);
        m_nodeStack.push(m_data);
    }

    void ReadStreamYaml::Close()
    {
        
    }

    void ReadStreamYaml::BeginMap(const std::string& key)
    {
        m_nodeStack.push(m_nodeStack.top()[key]);
    }

    void ReadStreamYaml::EndMap()
    {
        m_nodeStack.pop();
    }

    size_t ReadStreamYaml::BeginArray(const std::string& key)
    {
        m_nodeStack.push(m_nodeStack.top()[key]);
        return m_nodeStack.top().size();
    }

    void ReadStreamYaml::EndArray()
    {
        m_nodeStack.pop();
    }

    void ReadStreamYaml::EnterArray(size_t i)
    {
        m_nodeStack.push(m_nodeStack.top()[i]);
    }

    void ReadStreamYaml::LeaveArray()
    {
        m_nodeStack.pop();
    }

    void ReadStreamYaml::ReadValue(const std::string& key, bool& value)
    {
        value = m_nodeStack.top()[key].as<bool>();
    }

    void ReadStreamYaml::ReadValue(const std::string& key, int& value)
    {
        value = m_nodeStack.top()[key].as<int>();
    }

    void ReadStreamYaml::ReadValue(const std::string& key, uint32_t& value)
    {
        value = m_nodeStack.top()[key].as<uint32_t>();
    }

    void ReadStreamYaml::ReadValue(const std::string& key, float& value)
    {
        value = m_nodeStack.top()[key].as<float>();
    }

    void ReadStreamYaml::ReadValue(const std::string& key, glm::vec2& value)
    {
        value = m_nodeStack.top()[key].as<glm::vec2>();
    }

    void ReadStreamYaml::ReadValue(const std::string& key, glm::vec3& value)
    {
        value = m_nodeStack.top()[key].as<glm::vec3>();
    }

    void ReadStreamYaml::ReadValue(const std::string& key, glm::vec4& value)
    {
        value = m_nodeStack.top()[key].as<glm::vec4>();
    }

    void ReadStreamYaml::ReadValue(const std::string& key, glm::bvec4& value)
    {
        value = m_nodeStack.top()[key].as<glm::bvec4>();
    }

    void ReadStreamYaml::ReadValue(const std::string& key, glm::mat3& value)
    {
        value = m_nodeStack.top()[key].as<glm::mat3>();
    }

    void ReadStreamYaml::ReadValue(const std::string& key, glm::mat4& value)
    {
        value = m_nodeStack.top()[key].as<glm::mat4>();
    }

    void ReadStreamYaml::ReadValue(const std::string& key, std::string& value)
    {
        value = m_nodeStack.top()[key].as<std::string>();
    }

    void ReadStreamYaml::ReadValue(const std::string& key, std::vector<uint16_t>& value)
    {
        if (!m_nodeStack.top()[key].IsNull()) {
            for (const auto &v : m_nodeStack.top()[key]) {
                value.push_back(v.as<uint16_t>());
            }
        }
    }

    void ReadStreamYaml::ReadValue(const std::string& key, std::vector<glm::vec2>& value)
    {
        if (!m_nodeStack.top()[key].IsNull()) {
            for (const auto &v : m_nodeStack.top()[key]) {
                value.push_back(v.as<glm::vec2>());
            }
        }
    }

    void ReadStreamYaml::ReadValue(const std::string& key, std::vector<glm::vec3>& value)
    {
        if (!m_nodeStack.top()[key].IsNull()) {
            for (const auto &v : m_nodeStack.top()[key]) {
                value.push_back(v.as<glm::vec3>());
            }
        }
    }

    void ReadStreamYaml::ReadValue(const std::string& key, std::vector<uint8_t>& value)
    {
        std::string decode = base64_decode(m_nodeStack.top()[key].as<std::string>());
        value.insert(value.begin(), decode.begin(), decode.end());
    }

}
