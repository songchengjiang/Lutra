//
//  SceneParser.cpp
//  Lutra
//
//  Created by JasonCheng on 2021/1/11.
//

#include "SceneParser.h"
#include "SceneObjectParser.h"
#include "Scene.h"
#include "Stream.h"
#include "SpriteSystem.h"
#include "TransformSystem.h"
#include "MeshFilterSystem.h"
#include "SceneObject.h"
#include "Serializable.h"

namespace Lutra {

    SceneParser::SceneParser(const std::shared_ptr<Scene>& scene)
    : m_scene(scene)
    {
        
    }

    SceneParser::~SceneParser()
    {
        
    }

    void SceneParser::Serialize(WriteStream& stream)
    {
        stream.BeginArray("Systems");
        for (auto& system : m_scene->GetSystems()) {
            stream.WriteArrayElement(system->GetType());
        }
        stream.EndArray();
        
        stream.BeginArray("SceneObjectIDs");
        for (auto& so : m_scene->GetSceneObjects()) {
            if (so.HasComponent<Serializable>()) {
                stream.WriteArrayElement(so.GetComponent<Serializable>().UUID.str());
            }
        }
        stream.EndArray();
        
        stream.BeginArray("SceneObjects");
        for (auto& so : m_scene->GetSceneObjects()) {
            if (so.HasComponent<Serializable>()) {
                stream.BeginMap("");
                SceneObjectParser soParser(so);
                soParser.Serialize(stream);
                stream.EndMap();
            }
        }
        stream.EndArray();
    }

    void SceneParser::Deserialize(ReadStream& stream)
    {
        size_t size = stream.BeginArray("Systems");
        for (size_t i = 0; i < size; ++i) {
            stream.EnterArray(i);
            std::string type;
            stream.ReadArrayElement(type);
            if (type == TransformSystem::GetTypeStatic()) {
                m_scene->AppendSystem<TransformSystem>();
            }else if (type == SpriteSystem::GetTypeStatic()) {
                m_scene->AppendSystem<SpriteSystem>();
            }else if (type == MeshFilterSystem::GetTypeStatic()) {
                m_scene->AppendSystem<MeshFilterSystem>();
            }
            stream.LeaveArray();
        }
        stream.EndArray();
        
        size = stream.BeginArray("SceneObjectIDs");
        for (size_t i = 0; i < size; ++i) {
            stream.EnterArray(i);
            std::string uuid;
            stream.ReadArrayElement(uuid);
            auto so = m_scene->CreateSceneObject();
            so.AddComponent<Serializable>().UUID = sole::rebuild(uuid);
            stream.LeaveArray();
        }
        stream.EndArray();
        
        size = stream.BeginArray("SceneObjects");
        for (size_t i = 0; i < size; ++i) {
            stream.EnterArray(i);
            SceneObjectParser soParser(m_scene->GetSceneObjects()[i]);
            soParser.Deserialize(stream);
            stream.LeaveArray();
        }
        stream.EndArray();
    }

}
