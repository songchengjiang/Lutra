//
//  SceneBuilder.cpp
//  Editor
//
//  Created by JasonCheng on 2021/1/13.
//

#include "SceneBuilder.h"
#include "Resources/ResourceID.h"

namespace LutraEditor {

    std::shared_ptr<Lutra::Scene> SceneBuilder::BuildSimpleScene(const std::string& path)
    {
        auto scene = std::shared_ptr<Lutra::Scene>(new Lutra::Scene);
        scene->AppendSystem<Lutra::TransformSystem>();
        scene->AppendSystem<Lutra::SpriteSystem>();
        scene->AppendSystem<Lutra::MeshFilterSystem>();
        scene->AppendSystem<Lutra::MeshSystem>();
        
        auto cameraSO = scene->CreateSceneObject("Camera");
        auto& camera = cameraSO.AddComponent<Lutra::Camera>();
        auto renderTexture = Lutra::ResourceManager::Instance().CreateResource<Lutra::RenderTexture>(1024, 1024, Lutra::TextureFormat::RGBA8, Lutra::TextureFormat::D24S8);
        renderTexture->SetName(DEVICE_TEXTURE_RESOURCE_ID);
        Lutra::ResourceManager::Instance().SaveResource(path + "/" + renderTexture->GetName() + ".rt", renderTexture);
        camera.RenderTexture_ = renderTexture;
        
        cameraSO.GetComponent<Lutra::Transform>().Position.z = 10.0f;
        
        cameraSO.AddComponent<Lutra::Serializable>();
        return scene;
    }

}
