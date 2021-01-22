//
//  MaterialNodeFactory.hpp
//  Editor
//
//  Created by JasonCheng on 2021/1/19.
//

#ifndef MaterialNodeFactory_hpp
#define MaterialNodeFactory_hpp
#include "BlueprintFactory.h"
#include <string>

namespace LutraEditor {

    template<class T>
    class BlueprintFactoryImp : public BlueprintFactory
    {
    public:
        BlueprintFactoryImp() = default;
        virtual ~BlueprintFactoryImp() = default;
        
        virtual std::shared_ptr<BlueprintNode> Create() override
        {
            std::shared_ptr<BlueprintNode> node{new T()};
            return node;
        }
        
        virtual std::string GetType() const override { return T::Type(); }
        virtual std::string GetGroup() const override { return T::Group(); };
    };
}

#endif /* MaterialNodeFactory_hpp */
