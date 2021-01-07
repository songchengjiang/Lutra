//
//  MeshFilter.hpp
//  Lutra
//
//  Created by JasonCheng on 2021/1/4.
//

#ifndef MeshFilter_hpp
#define MeshFilter_hpp
#include <memory>

namespace Lutra {

    struct Mesh;
    struct MeshFilter
    {
        std::shared_ptr<Mesh> MeshPtr;
    };
}

#endif /* MeshFilter_hpp */
