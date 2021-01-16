//
//  MeshBuilder.hpp
//  Editor
//
//  Created by JasonCheng on 2021/1/13.
//

#ifndef MeshBuilder_hpp
#define MeshBuilder_hpp
#include <Lutra.h>

namespace LutraEditor {

    class PlaneMeshBuilder
    {
    public:
    
        static std::shared_ptr<Lutra::Mesh> Build(float width, float height);
    };

    class SphereMeshBuilder
    {
    public:
        
        static std::shared_ptr<Lutra::Mesh> Build(float radius, uint32_t sectorCount, uint32_t stackCount);
    };

    class GridMeshBuilder
    {
    public:
        
        static std::shared_ptr<Lutra::Mesh> Build(float width, float height, float step = 1.0f, const glm::vec4& color = glm::vec4(1.0), const glm::vec4& centerColor = glm::vec4(0.0f, 0.0f, 1.0f, 1.0f));
    };

    class ViewportMeshBuilder
    {
    public:
        
        static std::shared_ptr<Lutra::Mesh> Build(float fov, float aspectRadio, float orthographicSize, float zNear, float zFar, bool isProjectionType = false, const glm::vec4& color = glm::vec4(1.0));
    };


}

#endif /* MeshBuilder_hpp */
