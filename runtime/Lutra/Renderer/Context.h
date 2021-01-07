//
//  Context.h
//  Lutra
//
//  Created by JasonCheng on 2021/1/5.
//

#ifndef Context_h
#define Context_h
#include "Timestep.h"

namespace Lutra {

    class Graphic;
    struct Camera;
    struct FrameContext
    {
        Timestep Timestep_;
    };

    struct RenderContext
    {
        Graphic* Graphic_;
        Camera*  Camera_;
    };

}

#endif /* Context_h */
