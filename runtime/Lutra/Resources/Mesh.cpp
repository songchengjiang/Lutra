//
//  Mesh.cpp
//  Lutra
//
//  Created by JasonCheng on 2021/1/4.
//

#include "Mesh.h"
#include "Stream.h"


namespace Lutra {

    Mesh::Mesh()
    {
        m_resourceType = ResourceType::Mesh;
    }

    void Mesh::Serialize(WriteStream& stream)
    {
        Resource::Serialize(stream);
        stream.WriteValue("Vertices", Vertices);
        if (!Normals.empty())
            stream.WriteValue("Normals", Normals);
        if (!Tangents.empty())
            stream.WriteValue("Tangents", Tangents);
        if (!Texcoord0.empty())
            stream.WriteValue("Texcoord0", Texcoord0);
        if (!Texcoord1.empty())
            stream.WriteValue("Texcoord1", Texcoord1);
        
        stream.BeginArray("SubMeshes");
        for (auto &subMesh : SubMeshList) {
            stream.BeginMap("");
            stream.WriteValue("MaterialID", subMesh.MaterialIndex);
            stream.WriteValue("Indices", subMesh.Indices);
            stream.EndMap();
        }
        stream.EndArray();
    }

    void Mesh::Deserialize(ReadStream& stream)
    {
        Resource::Deserialize(stream);
        stream.ReadValue("Vertices", Vertices);
        stream.ReadValue("Normals", Normals);
        stream.ReadValue("Tangents", Tangents);
        stream.ReadValue("Texcoord0", Texcoord0);
        stream.ReadValue("Texcoord1", Texcoord1);
        
        size_t size = stream.BeginArray("SubMeshes");
        SubMeshList.resize(size);
        for (size_t i = 0; i < size; ++i) {
            stream.EnterArray(i);
            stream.ReadValue("MaterialID", SubMeshList[i].MaterialIndex);
            stream.ReadValue("Indices", SubMeshList[i].Indices);
            stream.LeaveArray();
        }
        stream.EndArray();
    }

}
