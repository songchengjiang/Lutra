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
        
        virtual void OnUpdate(const FrameContext& context) override;
    };

}

#endif /* MeshFilterSystem_hpp */
