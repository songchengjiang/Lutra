//
//  SceneBuilder.hpp
//  Editor
//
//  Created by JasonCheng on 2021/1/13.
//

#ifndef SceneBuilder_hpp
#define SceneBuilder_hpp
#include <Lutra.h>

namespace LutraEditor {

    class SceneBuilder
    {
    public:
        
        static std::shared_ptr<Lutra::Scene> BuildSimpleScene(const std::string& path);
    };

}

#endif /* SceneBuilder_hpp */
