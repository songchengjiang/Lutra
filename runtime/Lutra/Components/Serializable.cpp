//
//  Serializable.cpp
//  Lutra
//
//  Created by JasonCheng on 2021/1/12.
//

#include "Serializable.h"

namespace Lutra {

    Serializable::Serializable()
    {
        UUID = sole::uuid4();
    }
}
