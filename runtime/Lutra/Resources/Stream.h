//
//  Stream.hpp
//  Lutra
//
//  Created by JasonCheng on 2021/1/10.
//

#ifndef Stream_hpp
#define Stream_hpp
#include <glm.hpp>
#include <string>

namespace Lutra {

    class Stream
    {
    public:
        Stream() = default;
        virtual ~Stream() = default;
        
        virtual void Open() = 0;
        virtual void Close() = 0;
    };

    class WriteStream : public Stream
    {
    public:
        WriteStream() = default;
        virtual ~WriteStream() = default;
        
        virtual void BeginMap(const std::string& key) = 0;
        virtual void EndMap() = 0;
        virtual void BeginArray(const std::string& key) = 0;
        virtual void EndArray() = 0;
        virtual void WriteArrayElement(uint32_t value) = 0;
        virtual void WriteArrayElement(std::string value) = 0;
        virtual void WriteValue(const std::string& key, bool value) = 0;
        virtual void WriteValue(const std::string& key, int value) = 0;
        virtual void WriteValue(const std::string& key, uint32_t value) = 0;
        virtual void WriteValue(const std::string& key, float value) = 0;
        virtual void WriteValue(const std::string& key, const glm::vec2& value) = 0;
        virtual void WriteValue(const std::string& key, const glm::vec3& value) = 0;
        virtual void WriteValue(const std::string& key, const glm::vec4& value) = 0;
        virtual void WriteValue(const std::string& key, const glm::bvec4& value) = 0;
        virtual void WriteValue(const std::string& key, const glm::mat3& value) = 0;
        virtual void WriteValue(const std::string& key, const glm::mat4& value) = 0;
        virtual void WriteValue(const std::string& key, const std::string& value) = 0;
        virtual void WriteValue(const std::string& key, const std::vector<uint16_t>& value) = 0;
        virtual void WriteValue(const std::string& key, const std::vector<glm::vec2>& value) = 0;
        virtual void WriteValue(const std::string& key, const std::vector<glm::vec3>& value) = 0;
        virtual void WriteValue(const std::string& key, const std::vector<uint8_t>& value) = 0;
    };

    class ReadStream : public Stream
    {
    public:
        ReadStream() = default;
        virtual ~ReadStream() = default;
        
        virtual bool HasMap(const std::string& key) = 0;
        virtual void BeginMap(const std::string& key) = 0;
        virtual void EndMap() = 0;
        virtual bool HasArray(const std::string& key) = 0;
        virtual size_t BeginArray(const std::string& key) = 0;
        virtual void EndArray() = 0;
        virtual void EnterArray(size_t i) = 0;
        virtual void LeaveArray() = 0;
        virtual void ReadArrayElement(uint32_t& value) = 0;
        virtual void ReadArrayElement(std::string& value) = 0;
        virtual bool HasValue(const std::string& key) = 0;
        virtual void ReadValue(const std::string& key, bool& value) = 0;
        virtual void ReadValue(const std::string& key, int& value) = 0;
        virtual void ReadValue(const std::string& key, uint32_t& value) = 0;
        virtual void ReadValue(const std::string& key, float& value) = 0;
        virtual void ReadValue(const std::string& key, glm::vec2& value) = 0;
        virtual void ReadValue(const std::string& key, glm::vec3& value) = 0;
        virtual void ReadValue(const std::string& key, glm::vec4& value) = 0;
        virtual void ReadValue(const std::string& key, glm::mat3& value) = 0;
        virtual void ReadValue(const std::string& key, glm::mat4& value) = 0;
        virtual void ReadValue(const std::string& key, glm::bvec4& value) = 0;
        virtual void ReadValue(const std::string& key, std::string& value) = 0;
        virtual void ReadValue(const std::string& key, std::vector<uint16_t>& value) = 0;
        virtual void ReadValue(const std::string& key, std::vector<glm::vec2>& value) = 0;
        virtual void ReadValue(const std::string& key, std::vector<glm::vec3>& value) = 0;
        virtual void ReadValue(const std::string& key, std::vector<uint8_t>& value) = 0;
    };
}
#endif /* Stream_hpp */
