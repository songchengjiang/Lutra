//
//  Renderer.hpp
//  Lutra
//
//  Created by JasonCheng on 2021/1/4.
//

#ifndef Renderer_hpp
#define Renderer_hpp
#include <vector>

namespace Lutra {

    class Material;
    struct Renderer
    {
        Renderer() = default;
        virtual ~Renderer();
        
        std::vector<std::shared_ptr<Material>> Materials;
    };
}

#endif /* Renderer_hpp */
