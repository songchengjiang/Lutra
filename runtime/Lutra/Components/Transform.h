//
//  Transform.hpp
//  Lutra
//
//  Created by JasonCheng on 2021/1/4.
//

#ifndef Transform_hpp
#define Transform_hpp
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <gtx/quaternion.hpp>
#include <vector>

namespace Lutra {

    struct Transform
    {
        glm::vec3 Position{0.0f, 0.0f, 0.0f};
        glm::vec3 Rotation{0.0f, 0.0f, 0.0f};
        glm::vec3 Scale{1.0f, 1.0f, 1.0f};
        
        glm::mat4 LocalMat;
        glm::mat4 WorldMat;
    };
}

#endif /* Transform_hpp */
