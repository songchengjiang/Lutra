//
//  BlueprintFactory.hpp
//  Editor
//
//  Created by JasonCheng on 2021/1/19.
//

#ifndef BlueprintFactory_hpp
#define BlueprintFactory_hpp
#include "BlueprintNode.h"

namespace LutraEditor {

    class BlueprintFactory
    {
    public:
        BlueprintFactory() = default;
        virtual ~BlueprintFactory() = default;
        
        virtual std::shared_ptr<BlueprintNode> Create() = 0;
        
        virtual std::string GetType() const = 0;
        
        virtual std::string GetGroup() const = 0;
    };
}

#endif /* BlueprintFactory_hpp */
