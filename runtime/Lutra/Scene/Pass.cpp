//
//  Pass.cpp
//  Lutra
//
//  Created by JasonCheng on 2021/1/4.
//

#include "Pass.h"

namespace Lutra {

    Pass::Pass(const PassBuilder& builder)
    : m_shader(builder.VertexShader, builder.FragmentShader)
    {
        
    }
}
