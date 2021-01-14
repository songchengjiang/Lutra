//
//  Serialize.h
//  Lutra
//
//  Created by JasonCheng on 2021/1/12.
//

#ifndef Serialize_h
#define Serialize_h
#include "sole.hpp"

namespace Lutra {

    struct Serializable
    {
        Serializable();
        sole::uuid UUID;
    };
}

#endif /* Serialize_h */
