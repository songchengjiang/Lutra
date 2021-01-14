//
//  SceneObjectParser.hpp
//  Lutra
//
//  Created by JasonCheng on 2021/1/11.
//

#ifndef SceneObjectParser_hpp
#define SceneObjectParser_hpp
#include "Parser.h"
#include "SceneObject.h"

namespace Lutra {

    class SceneObjectParser : public Parser
    {
    public:
        SceneObjectParser(SceneObject so);
        virtual ~SceneObjectParser();
        
        virtual void Serialize(WriteStream& stream) override;
        virtual void Deserialize(ReadStream& stream) override;
        
    private:
        
        SceneObject m_sceneObject;
    };
}

#endif /* SceneObjectParser_hpp */
