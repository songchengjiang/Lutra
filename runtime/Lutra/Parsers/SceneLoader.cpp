//
//  SceneLoader.cpp
//  Lutra
//
//  Created by JasonCheng on 2021/1/11.
//

#include "SceneLoader.h"
#include "SceneParser.h"
#include "StreamYaml.h"
#include "Scene.h"
#include "SceneObject.h"

namespace Lutra {

    std::shared_ptr<Scene> SceneLoader::Load(const std::string& absPath)
    {
        std::shared_ptr<Scene> scene{new Scene};
        ReadStreamYaml stream(absPath);
        stream.Open();
        SceneParser sp(scene);
        sp.Deserialize(stream);
        stream.Close();
        return scene;
    }

    void SceneLoader::Save(const std::string& absPath, const std::shared_ptr<Scene>& scene)
    {
        WriteStreamYaml stream(absPath);
        stream.Open();
        SceneParser sp(scene);
        sp.Serialize(stream);
        stream.Close();
    }

}
