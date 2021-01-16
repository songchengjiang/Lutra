//
//  MeshBuilder.cpp
//  Editor
//
//  Created by JasonCheng on 2021/1/13.
//

#include "MeshBuilder.h"

namespace LutraEditor {

    std::shared_ptr<Lutra::Mesh> PlaneMeshBuilder::Build(float width, float height)
    {
        auto mesh = Lutra::ResourceManager::Instance().CreateResource<Lutra::Mesh>();
        mesh->Vertices.push_back(glm::vec3(0.0f, 0.0f, 0.0f));
        mesh->Vertices.push_back(glm::vec3(width, 0.0f, 0.0f));
        mesh->Vertices.push_back(glm::vec3(0.0f, height, 0.0f));
        mesh->Vertices.push_back(glm::vec3(width, height, 0.0f));
        
        mesh->Normals.push_back(glm::vec3(0.0f, 0.0f, 1.0f));
        mesh->Normals.push_back(glm::vec3(0.0f, 0.0f, 1.0f));
        mesh->Normals.push_back(glm::vec3(0.0f, 0.0f, 1.0f));
        mesh->Normals.push_back(glm::vec3(0.0f, 0.0f, 1.0f));
        
        mesh->Texcoord0.push_back(glm::vec2(0.0f, 0.0f));
        mesh->Texcoord0.push_back(glm::vec2(1.0f, 0.0f));
        mesh->Texcoord0.push_back(glm::vec2(0.0f, 1.0f));
        mesh->Texcoord0.push_back(glm::vec2(1.0f, 1.0f));
        
        mesh->SubMeshList.emplace_back(Lutra::SubMesh{Lutra::PrimitiveType::Triangle, 0, {0, 1, 2, 1, 3, 2}});
        return mesh;
    }

    std::shared_ptr<Lutra::Mesh> SphereMeshBuilder::Build(float radius, uint32_t sectorCount, uint32_t stackCount)
    {
        auto mesh = Lutra::ResourceManager::Instance().CreateResource<Lutra::Mesh>();
        
        float x, y, z, xy;                              // vertex position
        float nx, ny, nz, lengthInv = 1.0f / radius;    // vertex normal
        float s, t;                                     // vertex texCoord

        float sectorStep = 2 * glm::pi<float>() / sectorCount;
        float stackStep = glm::pi<float>() / stackCount;
        float sectorAngle, stackAngle;

        for(int i = 0; i <= stackCount; ++i)
        {
            stackAngle = glm::pi<float>() / 2 - i * stackStep;        // starting from pi/2 to -pi/2
            xy = radius * cosf(stackAngle);             // r * cos(u)
            z = radius * sinf(stackAngle);              // r * sin(u)

            // add (sectorCount+1) vertices per stack
            // the first and last vertices have same position and normal, but different tex coords
            for(int j = 0; j <= sectorCount; ++j)
            {
                sectorAngle = j * sectorStep;           // starting from 0 to 2pi

                // vertex position (x, y, z)
                x = xy * cosf(sectorAngle);             // r * cos(u) * cos(v)
                y = xy * sinf(sectorAngle);             // r * cos(u) * sin(v)
                mesh->Vertices.push_back(glm::vec3(x, z, y));

                // normalized vertex normal (nx, ny, nz)
                nx = x * lengthInv;
                ny = y * lengthInv;
                nz = z * lengthInv;
                mesh->Normals.push_back(glm::vec3(nx, nz, ny));

                // vertex tex coord (s, t) range between [0, 1]
                s = (float)j / sectorCount;
                t = (float)i / stackCount;
                mesh->Texcoord0.push_back(glm::vec2(s, t));
            }
        }
        
        Lutra::SubMesh subMesh;
        subMesh.Type = Lutra::PrimitiveType::Triangle;
        subMesh.MaterialIndex = 0;
        int k1, k2;
        for(int i = 0; i < stackCount; ++i)
        {
            k1 = i * (sectorCount + 1);     // beginning of current stack
            k2 = k1 + sectorCount + 1;      // beginning of next stack

            for(int j = 0; j < sectorCount; ++j, ++k1, ++k2)
            {
                // 2 triangles per sector excluding first and last stacks
                // k1 => k2 => k1+1
                if(i != 0)
                {
                    subMesh.Indices.push_back(k1);
                    subMesh.Indices.push_back(k2);
                    subMesh.Indices.push_back(k1 + 1);
                }

                // k1+1 => k2 => k2+1
                if(i != (stackCount-1))
                {
                    subMesh.Indices.push_back(k1 + 1);
                    subMesh.Indices.push_back(k2);
                    subMesh.Indices.push_back(k2 + 1);
                }
            }
        }
        
        mesh->SubMeshList.emplace_back(subMesh);
        return mesh;
    }

    std::shared_ptr<Lutra::Mesh> GridMeshBuilder::Build(float width, float height, float step, const glm::vec4& color, const glm::vec4& centerColor)
    {
        auto mesh = Lutra::ResourceManager::Instance().CreateResource<Lutra::Mesh>();
        
        float halfWidth = width * 0.5f;
        float halfHeight = height * 0.5f;
        
        for (float h = -halfHeight; h <= halfHeight; h += step) {
            for (float w = -halfWidth; w <= halfWidth; w += step) {
                mesh->Vertices.push_back(glm::vec3(w, 0.0f, h));
                if (w == 0.0f || h == 0.0f)
                    mesh->Colors.push_back(centerColor);
                else
                    mesh->Colors.push_back(color);
            }
        }
        
        Lutra::SubMesh subMesh;
        subMesh.Type = Lutra::PrimitiveType::Line;
        subMesh.MaterialIndex = 0;
        uint32_t wCount = width / step;
        uint32_t hCount = height / step;
        uint32_t vertexWidth = wCount + 1;
        for (uint32_t h = 0; h < hCount; ++h) {
            for (uint32_t w = 0; w < wCount; ++w) {
                subMesh.Indices.push_back(h * vertexWidth + w);
                subMesh.Indices.push_back(h * vertexWidth + w + 1);
                
                subMesh.Indices.push_back(h * vertexWidth + w + 1);
                subMesh.Indices.push_back((h + 1) * vertexWidth + w + 1);
                
                subMesh.Indices.push_back((h + 1) * vertexWidth + w + 1);
                subMesh.Indices.push_back((h + 1) * vertexWidth + w);
                
                subMesh.Indices.push_back((h + 1) * vertexWidth + w);
                subMesh.Indices.push_back(h * vertexWidth + w);
            }
        }
        mesh->SubMeshList.emplace_back(subMesh);
        return mesh;
    }

    std::shared_ptr<Lutra::Mesh> ViewportMeshBuilder::Build(float fov, float aspectRadio, float orthographicSize, float zNear, float zFar, bool isProjectionType, const glm::vec4& color)
    {
        auto mesh = Lutra::ResourceManager::Instance().CreateResource<Lutra::Mesh>();
        
        float zNearHeight, zNearWidth, zFarHeight, zFarWidth;
        if (isProjectionType) {
            zNearHeight = zNear * std::tan(glm::radians(fov));
            zNearWidth  = zNearHeight * aspectRadio;
            zFarHeight  = zFar * std::tan(glm::radians(fov));
            zFarWidth   = zFarHeight * aspectRadio;
        }else {
            zFarWidth = zNearWidth = orthographicSize * aspectRadio;
            zFarHeight = zNearHeight = orthographicSize;
        }
        
        //zNear
        mesh->Vertices.push_back(glm::vec3(-zNearWidth * 0.5f, -zNearHeight * 0.5f, -zNear)); mesh->Colors.push_back(color);
        mesh->Vertices.push_back(glm::vec3(zNearWidth * 0.5f, -zNearHeight * 0.5f, -zNear)); mesh->Colors.push_back(color);
        mesh->Vertices.push_back(glm::vec3(zNearWidth * 0.5f, -zNearHeight * 0.5f, -zNear)); mesh->Colors.push_back(color);
        mesh->Vertices.push_back(glm::vec3(zNearWidth * 0.5f, zNearHeight * 0.5f, -zNear)); mesh->Colors.push_back(color);
        mesh->Vertices.push_back(glm::vec3(zNearWidth * 0.5f, zNearHeight * 0.5f, -zNear)); mesh->Colors.push_back(color);
        mesh->Vertices.push_back(glm::vec3(-zNearWidth * 0.5f, zNearHeight * 0.5f, -zNear)); mesh->Colors.push_back(color);
        mesh->Vertices.push_back(glm::vec3(-zNearWidth * 0.5f, zNearHeight * 0.5f, -zNear)); mesh->Colors.push_back(color);
        mesh->Vertices.push_back(glm::vec3(-zNearWidth * 0.5f, -zNearHeight * 0.5f, -zNear)); mesh->Colors.push_back(color);
        
        //ZFar
        mesh->Vertices.push_back(glm::vec3(-zFarWidth * 0.5f, -zFarHeight * 0.5f, -zFar)); mesh->Colors.push_back(color);
        mesh->Vertices.push_back(glm::vec3(zFarWidth * 0.5f, -zFarHeight * 0.5f, -zFar)); mesh->Colors.push_back(color);
        mesh->Vertices.push_back(glm::vec3(zFarWidth * 0.5f, -zFarHeight * 0.5f, -zFar)); mesh->Colors.push_back(color);
        mesh->Vertices.push_back(glm::vec3(zFarWidth * 0.5f, zFarHeight * 0.5f, -zFar)); mesh->Colors.push_back(color);
        mesh->Vertices.push_back(glm::vec3(zFarWidth * 0.5f, zFarHeight * 0.5f, -zFar)); mesh->Colors.push_back(color);
        mesh->Vertices.push_back(glm::vec3(-zFarWidth * 0.5f, zFarHeight * 0.5f, -zFar)); mesh->Colors.push_back(color);
        mesh->Vertices.push_back(glm::vec3(-zFarWidth * 0.5f, zFarHeight * 0.5f, -zFar)); mesh->Colors.push_back(color);
        mesh->Vertices.push_back(glm::vec3(-zFarWidth * 0.5f, -zFarHeight * 0.5f, -zFar)); mesh->Colors.push_back(color);
        
        //Link
        mesh->Vertices.push_back(glm::vec3(-zNearWidth * 0.5f, -zNearHeight * 0.5f, -zNear)); mesh->Colors.push_back(color);
        mesh->Vertices.push_back(glm::vec3(-zFarWidth * 0.5f, -zFarHeight * 0.5f, -zFar)); mesh->Colors.push_back(color);
        mesh->Vertices.push_back(glm::vec3(zNearWidth * 0.5f, -zNearHeight * 0.5f, -zNear)); mesh->Colors.push_back(color);
        mesh->Vertices.push_back(glm::vec3(zFarWidth * 0.5f, -zFarHeight * 0.5f, -zFar)); mesh->Colors.push_back(color);
        mesh->Vertices.push_back(glm::vec3(zNearWidth * 0.5f, zNearHeight * 0.5f, -zNear)); mesh->Colors.push_back(color);
        mesh->Vertices.push_back(glm::vec3(zFarWidth * 0.5f, zFarHeight * 0.5f, -zFar)); mesh->Colors.push_back(color);
        mesh->Vertices.push_back(glm::vec3(-zNearWidth * 0.5f, zNearHeight * 0.5f, -zNear)); mesh->Colors.push_back(color);
        mesh->Vertices.push_back(glm::vec3(-zFarWidth * 0.5f, zFarHeight * 0.5f, -zFar)); mesh->Colors.push_back(color);
        
        mesh->Vertices.push_back(glm::vec3(-zFarWidth * 0.5f, -zFarHeight * 0.5f, -zFar)); mesh->Colors.push_back(glm::vec4(color.x, color.y, color.z, 0.5f));
        mesh->Vertices.push_back(glm::vec3(zFarWidth * 0.5f, -zFarHeight * 0.5f, -zFar)); mesh->Colors.push_back(glm::vec4(color.x, color.y, color.z, 0.5f));
        mesh->Vertices.push_back(glm::vec3(-zFarWidth * 0.5f, zFarHeight * 0.5f, -zFar)); mesh->Colors.push_back(glm::vec4(color.x, color.y, color.z, 0.5f));
        mesh->Vertices.push_back(glm::vec3(zFarWidth * 0.5f, zFarHeight * 0.5f, -zFar)); mesh->Colors.push_back(glm::vec4(color.x, color.y, color.z, 0.5f));
        
        
        Lutra::SubMesh subMesh;
        subMesh.Type = Lutra::PrimitiveType::Line;
        subMesh.MaterialIndex = 0;
        for (uint32_t i = 0; i < 24; ++i) {
            subMesh.Indices.push_back(i);
        }
        mesh->SubMeshList.emplace_back(subMesh);
        
        mesh->SubMeshList.emplace_back(Lutra::SubMesh{Lutra::PrimitiveType::Triangle, 0, {24, 25, 26, 25, 27, 26}});
        
        return mesh;
    }

}
