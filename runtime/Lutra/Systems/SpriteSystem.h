//
//  SpriteSystem.hpp
//  Lutra
//
//  Created by JasonCheng on 2021/1/5.
//

#ifndef SpriteSystem_hpp
#define SpriteSystem_hpp
#include "Systems/System.h"

namespace Lutra {
    
    class Scene;
    class SpriteSystem : public System
    {
    public:
        
        SpriteSystem(Scene* scene);
        virtual ~SpriteSystem();
        
        static std::string GetTypeStatic() { return "SpriteSystem"; }
        
        virtual std::string GetType() const override { return "SpriteSystem"; }
        
        virtual void OnUpdate(const FrameContext& context) override;
        virtual void OnRender(const RenderContext& context) override;
    };

}

#endif /* SpriteSystem_hpp */
