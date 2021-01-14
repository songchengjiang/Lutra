//
//  Camera.hpp
//  Lutra
//
//  Created by JasonCheng on 2021/1/4.
//

#ifndef Camera_hpp
#define Camera_hpp
#include <glm.hpp>
#include <memory>

namespace Lutra {

    class RenderTexture;

    struct Viewport
    {
        float X;
        float Y;
        float Width;
        float Height;
    };


    struct Camera
    {
        enum class ProjectionType {
            Perspective = 0,
            Orthographic
        };
        
        ProjectionType ProjType{ProjectionType::Perspective};
        float Fov{60.0f};
        float AspectRadio{1.0f};
        float OrthographicSize{1.0f};
        float ZNear{1.0f};
        float ZFar{100.0f};
        
        Viewport Viewport_{0.0f, 0.0f, 1.0f, 1.0f};
        std::shared_ptr<RenderTexture> RenderTexture_;
        
        glm::mat4 ViewMat;
        glm::mat4 ProjectMat;
    };
}

#endif /* Camera_hpp */
