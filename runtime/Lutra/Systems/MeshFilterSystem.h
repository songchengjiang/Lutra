//
//  MeshFilterSystem.hpp
//  Lutra
//
//  Created by JasonCheng on 2021/1/7.
//

#ifndef MeshFilterSystem_hpp
#define MeshFilterSystem_hpp

#include "Systems/System.h"

namespace Lutra {
    
    class Scene;
    class MeshFilterSystem : public System
    {
    public:
        
        MeshFilterSystem(Scene* scene);
        virtual ~MeshFilterSystem();
        
        static std::string GetTypeStatic() { return "MeshFilterSystem"; }
        
        virtual std::string GetType() const override { return "MeshFilterSystem"; }
        
        virtual void OnUpdate(const FrameContext& context) override;
    };

}

#endif /* MeshFilterSystem_hpp */
