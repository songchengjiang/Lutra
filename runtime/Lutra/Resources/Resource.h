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
        Texture2D = 0,
        RenderTexture,
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
        
        void SetName(const std::string& name) { m_name = name; }
        const std::string& GetName() const { return m_name; }
        
    protected:
      
        ResourceType m_resourceType;
        sole::uuid m_uuid;
        std::string m_name;
        friend class ResourceManager;
    };
}

#endif /* Resource_hpp */
