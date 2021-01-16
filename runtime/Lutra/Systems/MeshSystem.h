//
//  MeshSystem.hpp
//  Lutra
//
//  Created by JasonCheng on 2021/1/14.
//

#ifndef MeshSystem_hpp
#define MeshSystem_hpp
#include "Systems/System.h"

namespace Lutra {
    
    class MeshSystem : public System
    {
    public:
        
        MeshSystem(Scene* scene);
        virtual ~MeshSystem();
        
        static std::string GetTypeStatic() { return "MeshSystem"; }

        virtual std::string GetType() const override { return "MeshSystem"; }
        
        virtual void OnUpdate(const FrameContext& context) override;
        virtual void OnRender(const RenderContext& context) override;
    };

}

#endif /* MeshSystem_hpp */
