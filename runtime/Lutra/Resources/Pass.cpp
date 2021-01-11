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
    , m_blendMode(builder.BdMode)
    , m_cullMode(builder.ClMode)
    , m_depthTest(builder.DepthTest)
    , m_depthWrite(builder.DepthWrite)
    , m_twoSided(builder.TwoSided)
    , m_colorMask(builder.ColorMask)
    {
        
    }
}
