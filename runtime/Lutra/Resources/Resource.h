//
//  Resource.hpp
//  Lutra
//
//  Created by JasonCheng on 2021/1/10.
//

#ifndef Resource_hpp
#define Resource_hpp
#include "sole.hpp"

namespace Lutra {

    class WriteStream;
    class ReadStream;
    enum class ResourceType
    {
        Texture = 0,
        Material,
        Mesh
    };

    class Resource
    {
    public:
        Resource();
        virtual ~Resource();
        
        virtual void Serialize(WriteStream& stream);
        virtual void Deserialize(ReadStream& stream);
        
        ResourceType GetResourceType() const { return m_resourceType; }
        const sole::uuid& GetUUID() const { return m_uuid; }
        
    protected:
      
        ResourceType m_resourceType;
        sole::uuid m_uuid;
        friend class ResourceManager;
    };
}

#endif /* Resource_hpp */
