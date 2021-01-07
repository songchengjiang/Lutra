//
//  Buffer.hpp
//  Lutra
//
//  Created by JasonCheng on 2021/1/6.
//

#ifndef Buffer_hpp
#define Buffer_hpp
#include "LutraAssert.h"
#include <memory>
#include <string>
#include <vector>

namespace Lutra {

    enum class BufferDataType
    {
        None = 0,
        Float,
        Float2,
        Float3,
        Float4,
        Mat3,
        Mat4,
        Int,
        Int2,
        Int3,
        Int4,
        Bool
    };

    static uint32_t BufferDataTypeSize(BufferDataType type)
    {
        switch (type)
        {
            case BufferDataType::Float:    return 4;
            case BufferDataType::Float2:   return 4 * 2;
            case BufferDataType::Float3:   return 4 * 3;
            case BufferDataType::Float4:   return 4 * 4;
            case BufferDataType::Mat3:     return 4 * 3 * 3;
            case BufferDataType::Mat4:     return 4 * 4 * 4;
            case BufferDataType::Int:      return 4;
            case BufferDataType::Int2:     return 4 * 2;
            case BufferDataType::Int3:     return 4 * 3;
            case BufferDataType::Int4:     return 4 * 4;
            case BufferDataType::Bool:     return 1;
            default: break;
        }

        LT_CORE_ASSERT(false, "Unknown ShaderDataType!");
        return 0;
    }

    struct BufferElement
    {
        std::string Name;
        BufferDataType Type;
        uint32_t Size;
        size_t Offset;
        bool Normalized;

        BufferElement() = default;

        BufferElement(BufferDataType type, const std::string& name, bool normalized = false)
            : Name(name), Type(type), Size(BufferDataTypeSize(type)), Offset(0), Normalized(normalized)
        {
        }

        uint32_t GetComponentCount() const
        {
            switch (Type)
            {
                case BufferDataType::Float:   return 1;
                case BufferDataType::Float2:  return 2;
                case BufferDataType::Float3:  return 3;
                case BufferDataType::Float4:  return 4;
                case BufferDataType::Mat3:    return 3; // 3* float3
                case BufferDataType::Mat4:    return 4; // 4* float4
                case BufferDataType::Int:     return 1;
                case BufferDataType::Int2:    return 2;
                case BufferDataType::Int3:    return 3;
                case BufferDataType::Int4:    return 4;
                case BufferDataType::Bool:    return 1;
                default: break;
            }

            LT_CORE_ASSERT(false, "Unknown ShaderDataType!");
            return 0;
        }
    };

    class BufferLayout
    {
    public:
        BufferLayout() {}

        BufferLayout(const std::vector<BufferElement>& elements)
            : m_elements(elements)
        {
            CalculateOffsetsAndStride();
        }

        uint32_t GetStride() const { return m_stride; }
        const std::vector<BufferElement>& GetElements() const { return m_elements; }

        std::vector<BufferElement>::iterator begin() { return m_elements.begin(); }
        std::vector<BufferElement>::iterator end() { return m_elements.end(); }
        std::vector<BufferElement>::const_iterator begin() const { return m_elements.begin(); }
        std::vector<BufferElement>::const_iterator end() const { return m_elements.end(); }
    private:
        void CalculateOffsetsAndStride()
        {
            size_t offset = 0;
            m_stride = 0;
            for (auto& element : m_elements)
            {
                element.Offset = offset;
                offset += element.Size;
                m_stride += element.Size;
            }
        }
    private:
        std::vector<BufferElement> m_elements;
        uint32_t m_stride = 0;
    };

    class VertexBuffer
    {
    public:
        virtual ~VertexBuffer() = default;

        virtual void Bind() const = 0;
        virtual void Unbind() const = 0;

        virtual void SetData(float* vertices, uint32_t size) = 0;

        virtual const BufferLayout& GetLayout() const = 0;
        virtual void SetLayout(const BufferLayout& layout) = 0;
    };

    class IndexBuffer
    {
    public:
        virtual ~IndexBuffer() = default;

        virtual void Bind() const = 0;
        virtual void Unbind() const = 0;
        
        virtual uint32_t GetSize() const = 0;
    };

}

#endif /* Buffer_hpp */
