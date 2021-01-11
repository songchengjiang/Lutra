//
//  MouseCode.h
//  Lutra
//
//  Created by JasonCheng on 2021/1/8.
//

#ifndef MouseCode_h
#define MouseCode_h

namespace Lutra
{
    using MouseCode = uint16_t;

    namespace Mouse
    {
        enum : MouseCode
        {
            // From glfw3.h
            Button0                = 0,
            Button1                = 1,
            Button2                = 2,
            Button3                = 3,
            Button4                = 4,
            Button5                = 5,
            Button6                = 6,
            Button7                = 7,

            ButtonLast             = Button7,
            ButtonLeft             = Button0,
            ButtonRight            = Button1,
            ButtonMiddle           = Button2
        };
    }
}

#endif /* MouseCode_h */
