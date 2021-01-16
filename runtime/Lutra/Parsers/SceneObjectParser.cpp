//
//  SceneObjectParser.cpp
//  Lutra
//
//  Created by JasonCheng on 2021/1/11.
//

#include "SceneObjectParser.h"
#include "Stream.h"
#include "ComponentParser.h"
#include "Tag.h"
#include "Transform.h"
#include "Camera.h"
#include "SpriteRenderer.h"
#include "MeshRenderer.h"
#include "MeshFilter.h"
#include "Serializable.h"
#include "SceneObjectDelegate.h"

namespace Lutra {

    SceneObjectParser::SceneObjectParser(SceneObject so)
    : m_sceneObject(so)
    {
        
    }

    SceneObjectParser::~SceneObjectParser()
    {
        
    }

    void SceneObjectParser::Serialize(WriteStream& stream)
    {
        stream.BeginArray("Components");
        if (m_sceneObject.HasComponent<Tag>()) {
            stream.BeginMap("");
            stream.WriteValue("Type", (int)ComponentType::Tag);
            TagParser parser(m_sceneObject.GetComponent<Tag>());
            parser.Serialize(stream);
            stream.EndMap();
        }
        if (m_sceneObject.HasComponent<Transform>()) {
            stream.BeginMap("");
            stream.WriteValue("Type", (int)ComponentType::Transform);
            TransformParser parser(m_sceneObject.GetComponent<Transform>());
            parser.Serialize(stream);
            stream.EndMap();
        }
        if (m_sceneObject.HasComponent<SceneObjectDelegate>()) {
            stream.BeginMap("");
            stream.WriteValue("Type", (int)ComponentType::SceneObjectDelegate);
            SceneObjectDelegateParser parser(m_sceneObject.GetComponent<SceneObjectDelegate>());
            parser.Serialize(stream);
            stream.EndMap();
        }
        if (m_sceneObject.HasComponent<Camera>()) {
            stream.BeginMap("");
            stream.WriteValue("Type", (int)ComponentType::Camera);
            CameraParser parser(m_sceneObject.GetComponent<Camera>());
            parser.Serialize(stream);
            stream.EndMap();
        }
        if (m_sceneObject.HasComponent<SpriteRenderer>()) {
            stream.BeginMap("");
            stream.WriteValue("Type", (int)ComponentType::SpriteRenderer);
            SpriteRendererParser parser(m_sceneObject.GetComponent<SpriteRenderer>());
            parser.Serialize(stream);
            stream.EndMap();
        }
        if (m_sceneObject.HasComponent<MeshRenderer>()) {
            stream.BeginMap("");
            stream.WriteValue("Type", (int)ComponentType::MeshRenderer);
            MeshRendererParser parser(m_sceneObject.GetComponent<MeshRenderer>());
            parser.Serialize(stream);
            stream.EndMap();
        }
        if (m_sceneObject.HasComponent<MeshFilter>()) {
            stream.BeginMap("");
            stream.WriteValue("Type", (int)ComponentType::MeshFilter);
            MeshFilterParser parser(m_sceneObject.GetComponent<MeshFilter>());
            parser.Serialize(stream);
            stream.EndMap();
        }
        if (m_sceneObject.HasComponent<Serializable>()) {
            stream.BeginMap("");
            stream.WriteValue("Type", (int)ComponentType::Serializable);
            stream.EndMap();
        }
        stream.EndArray();
    }

    void SceneObjectParser::Deserialize(ReadStream& stream)
    {
        size_t size = stream.BeginArray("Components");
        for (size_t i = 0; i < size; ++i) {
            stream.EnterArray(i);
            int type;
            stream.ReadValue("Type", type);
            switch ((ComponentType)type) {
                case ComponentType::Tag :
                {
                    TagParser parser(m_sceneObject.GetComponent<Tag>());
                    parser.Deserialize(stream);
                }
                    break;
                case ComponentType::Transform :
                {
                    TransformParser parser(m_sceneObject.GetComponent<Transform>());
                    parser.Deserialize(stream);
                }
                    break;
                case ComponentType::SceneObjectDelegate :
                {
                    SceneObjectDelegateParser parser(m_sceneObject.GetComponent<SceneObjectDelegate>());
                    parser.Deserialize(stream);
                }
                    break;
                case ComponentType::Camera :
                {
                    CameraParser parser(m_sceneObject.AddComponent<Camera>());
                    parser.Deserialize(stream);
                }
                    break;
                case ComponentType::SpriteRenderer :
                {
                    SpriteRendererParser parser(m_sceneObject.AddComponent<SpriteRenderer>());
                    parser.Deserialize(stream);
                }
                    break;
                case ComponentType::MeshRenderer :
                {
                    MeshRendererParser parser(m_sceneObject.AddComponent<MeshRenderer>());
                    parser.Deserialize(stream);
                }
                    break;
                case ComponentType::MeshFilter :
                {
                    MeshFilterParser parser(m_sceneObject.AddComponent<MeshFilter>());
                    parser.Deserialize(stream);
                }
                    break;
                    
                default:
                    break;
            }
            
            stream.LeaveArray();
        }
        stream.EndArray();
    }

}
