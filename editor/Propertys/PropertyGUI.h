//
//  PropertyGUI.hpp
//  Editor
//
//  Created by JasonCheng on 2021/1/9.
//

#ifndef PropertyGUI_hpp
#define PropertyGUI_hpp

namespace LutraEditor {

    class PropertyGUI
    {
    public:
        
        PropertyGUI() = default;
        virtual ~PropertyGUI() = default;
        
        virtual void OnGUI() = 0;
        
    };

}

#endif /* PropertyGUI_hpp */
