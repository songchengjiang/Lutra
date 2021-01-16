//
//  Tag.hpp
//  Lutra
//
//  Created by JasonCheng on 2021/1/9.
//

#ifndef Tag_hpp
#define Tag_hpp
#include <string>

namespace Lutra {

    struct Tag
    {
        uint32_t    Tags{0};
        uint32_t    VisibleTag{1};
        std::string Name;
    };
}

#endif /* Tag_hpp */
