//
//  MathHelper.hpp
//  Editor
//
//  Created by JasonCheng on 2021/1/15.
//

#ifndef MathHelper_hpp
#define MathHelper_hpp
#include <Lutra.h>

namespace LutraEditor {

    class MathHelper
    {
    public:
        
        static bool decompose(const glm::mat4& m, glm::vec3* translation, glm::quat* rotation, glm::vec3* scale);
    };

}

#endif /* MathHelper_hpp */
