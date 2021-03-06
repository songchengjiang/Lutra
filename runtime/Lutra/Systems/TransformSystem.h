//
//  TransformSystem.hpp
//  Lutra
//
//  Created by JasonCheng on 2021/1/5.
//

#ifndef TransformSystem_hpp
#define TransformSystem_hpp
#include "Systems/System.h"

namespace Lutra {
    
    class Scene;
    class TransformSystem : public System
    {
    public:
        
        TransformSystem(Scene* scene);
        virtual ~TransformSystem();
        
        static std::string GetTypeStatic() { return "TransformSystem"; }
        
        virtual std::string GetType() const override { return "TransformSystem"; }
        
        virtual void OnUpdate(const FrameContext& context) override;
        
    };

}

#endif /* TransformSystem_hpp */
