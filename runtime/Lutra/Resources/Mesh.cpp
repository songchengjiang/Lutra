//
//  Mesh.cpp
//  Lutra
//
//  Created by JasonCheng on 2021/1/4.
//

#include "Mesh.h"
#include "Stream.h"


namespace Lutra {

    void Mesh::Serialize(WriteStream& stream)
    {
        Resource::Serialize(stream);
        stream.WriteValue("Vertices", Vertices);
        if (!Normals.empty())
            stream.WriteValue("Normals", Normals);
        if (!Tangents.empty())
            stream.WriteValue("Tangents", Tangents);
        if (!Colors.empty())
            stream.WriteValue("Colors", Colors);
        if (!Texcoord0.empty())
            stream.WriteValue("Texcoord0", Texcoord0);
        if (!Texcoord1.empty())
            stream.WriteValue("Texcoord1", Texcoord1);
        
        stream.BeginArray("SubMeshes");
        for (auto &subMesh : SubMeshList) {
            stream.BeginMap("");
            stream.WriteValue("PrimitiveType", (int)subMesh.Type);
            stream.WriteValue("MaterialID", subMesh.MaterialIndex);
            stream.WriteValue("Indices", subMesh.Indices);
            stream.BeginMap("BoundingBox");
            stream.WriteValue("Min", subMesh.BBox.Min());
            stream.WriteValue("Max", subMesh.BBox.Min());
            stream.EndMap();
            stream.EndMap();
        }
        stream.EndArray();
    }

    void Mesh::Deserialize(ReadStream& stream)
    {
        Resource::Deserialize(stream);
        stream.ReadValue("Vertices", Vertices);
        if (stream.HasValue("Normals"))
            stream.ReadValue("Normals", Normals);
        if (stream.HasValue("Tangents"))
            stream.ReadValue("Tangents", Tangents);
        if (stream.HasValue("Colors"))
            stream.ReadValue("Colors", Colors);
        if (stream.HasValue("Texcoord0"))
            stream.ReadValue("Texcoord0", Texcoord0);
        if (stream.HasValue("Texcoord1"))
            stream.ReadValue("Texcoord1", Texcoord1);
        
        size_t size = stream.BeginArray("SubMeshes");
        SubMeshList.resize(size);
        for (size_t i = 0; i < size; ++i) {
            stream.EnterArray(i);
            int type; stream.ReadValue("PrimitiveType", type); SubMeshList[i].Type = (PrimitiveType)type;
            stream.ReadValue("MaterialID", SubMeshList[i].MaterialIndex);
            stream.ReadValue("Indices", SubMeshList[i].Indices);
            stream.BeginMap("BoundingBox");
            stream.ReadValue("Min", SubMeshList[i].BBox.Min());
            stream.ReadValue("Max", SubMeshList[i].BBox.Max());
            stream.EndMap();
            stream.LeaveArray();
        }
        stream.EndArray();
    }

}
