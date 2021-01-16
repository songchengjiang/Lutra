//
//  Mesh.hpp
//  Lutra
//
//  Created by JasonCheng on 2021/1/4.
//

#ifndef Mesh_hpp
#define Mesh_hpp
#include <vector>
#include <glm.hpp>
#include "Resource.h"
#include "Base/BoundingBox.h"

namespace Lutra {

    enum class PrimitiveType
    {
        Line = 0,
        Triangle
    };

    struct SubMesh
    {
        PrimitiveType         Type;
        uint32_t              MaterialIndex;
        std::vector<uint16_t> Indices;
        BoundingBox           BBox;
    };

    struct Mesh : public Resource
    {
        virtual void Serialize(WriteStream& stream) override;
        virtual void Deserialize(ReadStream& stream) override;
        
        std::vector<glm::vec3> Vertices;
        std::vector<glm::vec3> Normals;
        std::vector<glm::vec3> Tangents;
        std::vector<glm::vec4> Colors;
        std::vector<glm::vec2> Texcoord0;
        std::vector<glm::vec2> Texcoord1;
        
        bool MarkForUpdate = true;
        
        std::vector<SubMesh> SubMeshList;
        
        //Auto Generating
        std::vector<float> SequentialBuffer;
    };
}

#endif /* Mesh_hpp */
