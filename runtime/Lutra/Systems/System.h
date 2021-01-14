//
//  System.hpp
//  Lutra
//
//  Created by JasonCheng on 2021/1/5.
//

#ifndef System_hpp
#define System_hpp
#include "entt.hpp"
#include <string>

namespace Lutra {
    
    class Scene;
    struct FrameContext;
    struct RenderContext;
    class System
    {
    public:
        
        System(Scene* scene);
        virtual ~System();
        
        virtual std::string GetType() const = 0;
        
        virtual void OnUpdate(const FrameContext& context) = 0;
        virtual void OnRender(const RenderContext& context){};
    
    protected:
        
        entt::registry& registry();
        
    protected:
        
        Scene*      m_scene{nullptr};
    };

}

#endif /* System_hpp */
