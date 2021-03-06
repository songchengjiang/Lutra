//
//  VertexArray.hpp
//  Lutra
//
//  Created by JasonCheng on 2021/1/6.
//

#ifndef VertexArray_hpp
#define VertexArray_hpp
#include "DeviceResource.h"

#include <memory>

namespace Lutra {

    class VertexBuffer;
    class IndexBuffer;
    class Program;
    class VertexArray : public DeviceResource
    {
    public:
        virtual ~VertexArray() = default;

        virtual void Bind(const std::shared_ptr<Program>& program) = 0;
        virtual void Unbind() = 0;

        virtual void AddVertexBuffer(const std::shared_ptr<VertexBuffer>& vertexBuffer) = 0;
        virtual void SetIndexBuffer(const std::shared_ptr<IndexBuffer>& indexBuffer) = 0;

        virtual const std::vector<std::shared_ptr<VertexBuffer>>& GetVertexBuffers() const = 0;
        virtual const std::shared_ptr<IndexBuffer>& GetIndexBuffer() const = 0;

    };

}

#endif /* VertexArray_hpp */
