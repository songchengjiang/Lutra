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

    class UnlintMaterialBuilder
    {
    public:
        
        static std::shared_ptr<Lutra::Material> Build();
    };

    class UnlintColorMaterialBuilder
    {
    public:
        
        static std::shared_ptr<Lutra::Material> Build();
    };

}

#endif /* MaterialBuilder_hpp */
