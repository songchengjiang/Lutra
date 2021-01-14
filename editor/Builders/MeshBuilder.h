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

    class MeshBuilder
    {
    public:
        
        static std::shared_ptr<Lutra::Mesh> BuildPlane();
        static std::shared_ptr<Lutra::Mesh> BuildBox();
        static std::shared_ptr<Lutra::Mesh> BuildSphere();
    };

}

#endif /* MeshBuilder_hpp */
