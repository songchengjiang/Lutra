//
//  SceneParser.hpp
//  Lutra
//
//  Created by JasonCheng on 2021/1/11.
//

#ifndef SceneParser_hpp
#define SceneParser_hpp
#include "Parser.h"
#include <memory>

namespace Lutra {

    class Scene;
    class SceneParser : public Parser
    {
    public:
        SceneParser(const std::shared_ptr<Scene>& scene);
        virtual ~SceneParser();
        
        virtual void Serialize(WriteStream& stream) override;
        virtual void Deserialize(ReadStream& stream) override;
        
    private:
        
        std::shared_ptr<Scene> m_scene;
    };
}

#endif /* SceneParser_hpp */
