//
//  SpriteRenderer.hpp
//  Lutra
//
//  Created by JasonCheng on 2021/1/4.
//

#ifndef SpriteRenderer_hpp
#define SpriteRenderer_hpp
#include <glm.hpp>
#include "Components/Renderer.h"

namespace Lutra {

    struct SpriteRenderer : public Renderer
    {
        glm::vec2 Size{1.0f, 1.0f};
        glm::vec2 Anchor{0.5f, 0.5f};
    };
}

#endif /* SpriteRenderer_hpp */
