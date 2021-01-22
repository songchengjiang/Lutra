//
//  ResourceFactory.h
//  Lutra
//
//  Created by JasonCheng on 2021/1/15.
//

#ifndef ResourceFactory_h
#define ResourceFactory_h
#include "Resource.h"

namespace Lutra {

    class ResourceFactory
    {
    public:
        virtual ~ResourceFactory() = default;
        virtual std::shared_ptr<Resource> Create() = 0;
    };

    template<class T>
    class ResourceFactoryImp : public ResourceFactory
    {
    public:
        
        virtual std::shared_ptr<Resource> Create() override
        {
            return std::shared_ptr<Resource>(new T);
        }
    };
}

#endif /* ResourceFactory_h */
