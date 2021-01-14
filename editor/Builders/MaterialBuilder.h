//
//  MaterialBuilder.hpp
//  Editor
//
//  Created by JasonCheng on 2021/1/13.
//

#ifndef MaterialBuilder_hpp
#define MaterialBuilder_hpp
#include <Lutra.h>

namespace LutraEditor {

    class MaterialBuilder
    {
    public:
        
        static std::shared_ptr<Lutra::Material> BuildUnlint();
    };

}

#endif /* MaterialBuilder_hpp */
