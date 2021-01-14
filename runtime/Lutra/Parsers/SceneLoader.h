//
//  SceneLoader.hpp
//  Lutra
//
//  Created by JasonCheng on 2021/1/11.
//

#ifndef SceneLoader_hpp
#define SceneLoader_hpp
#include <memory>

namespace Lutra {

    class Scene;
    class SceneLoader
    {
    public:
        SceneLoader() = default;
        ~SceneLoader() = default;
    
        std::shared_ptr<Scene> Load(const std::string& absPath);
        void Save(const std::string& absPath, const std::shared_ptr<Scene>& scene);
    };
}

#endif /* SceneLoader_hpp */
