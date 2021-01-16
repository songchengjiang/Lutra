//
//  ComponentParser.cpp
//  Lutra
//
//  Created by JasonCheng on 2021/1/11.
//

#include "ComponentParser.h"
#include "sole.hpp"
#include "Stream.h"
#include "Tag.h"
#include "Transform.h"
#include "Camera.h"
#include "SpriteRenderer.h"
#include "MeshRenderer.h"
#include "MeshFilter.h"
#include "Mesh.h"
#include "SceneObjectDelegate.h"
#include "Serializable.h"
#include "Texture.h"
#include "Material.h"
#include "Scene.h"
#include "ResourceManager.h"
#include <sole.hpp>

namespace Lutra {

    void TagParser::Serialize(WriteStream& stream)
    {
        stream.WriteValue("Tag", m_tag.Tags);
        stream.WriteValue("Name", m_tag.Name);
        stream.WriteValue("VisibleTag", m_tag.VisibleTag);
    }

    void TagParser::Deserialize(ReadStream& stream)
    {
        stream.ReadValue("Tag", m_tag.Tags);
        stream.ReadValue("Name", m_tag.Name);
        stream.ReadValue("VisibleTag", m_tag.VisibleTag);
    }

    void TransformParser::Serialize(WriteStream& stream)
    {
        stream.WriteValue("Position", m_transform.Position);
        stream.WriteValue("Rotation", m_transform.Rotation);
        stream.WriteValue("Scale", m_transform.Scale);
    }

    void TransformParser::Deserialize(ReadStream& stream)
    {
        stream.ReadValue("Position", m_transform.Position);
        stream.ReadValue("Rotation", m_transform.Rotation);
        stream.ReadValue("Scale", m_transform.Scale);
    }

    void CameraParser::Serialize(WriteStream& stream)
    {
        stream.WriteValue("IsMain", m_camera.IsMain);
        stream.WriteValue("VisibleMask", m_camera.VisibleMask);
        stream.WriteValue("ProjectionType", (int)m_camera.ProjType);
        stream.WriteValue("Fov", m_camera.Fov);
        stream.WriteValue("AspectRadio", m_camera.AspectRadio);
        stream.WriteValue("OrthographicSize", m_camera.OrthographicSize);
        stream.WriteValue("ZNear", m_camera.ZNear);
        stream.WriteValue("ZFar", m_camera.ZFar);
        stream.WriteValue("Viewport", glm::vec4(m_camera.Viewport_.X, m_camera.Viewport_.Y, m_camera.Viewport_.Width, m_camera.Viewport_.Height));
        
        if (m_camera.RenderTexture_ != nullptr) {
            stream.WriteValue("RenderTexture", m_camera.RenderTexture_->GetUUID().str());
        }
    }

    void CameraParser::Deserialize(ReadStream& stream)
    {
        stream.ReadValue("IsMain", m_camera.IsMain);
        stream.ReadValue("VisibleMask", m_camera.VisibleMask);
        int type; stream.ReadValue("ProjectionType", type); m_camera.ProjType = (Camera::ProjectionType)type;
        stream.ReadValue("Fov", m_camera.Fov);
        stream.ReadValue("AspectRadio", m_camera.AspectRadio);
        stream.ReadValue("OrthographicSize", m_camera.OrthographicSize);
        stream.ReadValue("ZNear", m_camera.ZNear);
        stream.ReadValue("ZFar", m_camera.ZFar);
        glm::vec4 viewport; stream.ReadValue("Viewport", viewport); m_camera.Viewport_ = {viewport.x, viewport.y, viewport.z, viewport.w};
        if (stream.HasValue("RenderTexture")) {
            std::string uuid;
            stream.ReadValue("RenderTexture", uuid);
            m_camera.RenderTexture_ = ResourceManager::Instance().LoadResource<RenderTexture>(sole::rebuild(uuid));
        }
    }

    void SpriteRendererParser::Serialize(WriteStream& stream)
    {
        stream.BeginArray("Materials");
        for (auto& mat : m_spriteRenderer.Materials) {
            stream.WriteArrayElement(mat != nullptr? mat->GetUUID().str(): "");
        }
        stream.EndArray();
        stream.WriteValue("Size", m_spriteRenderer.Size);
        stream.WriteValue("Anchor", m_spriteRenderer.Anchor);
    }

    void SpriteRendererParser::Deserialize(ReadStream& stream)
    {
        size_t size = stream.BeginArray("Materials");
        for (size_t i = 0; i < size; ++i) {
            stream.EnterArray(i);
            std::string uuid;
            stream.ReadArrayElement(uuid);
            m_spriteRenderer.Materials.push_back(ResourceManager::Instance().LoadResource<Material>(sole::rebuild(uuid)));
            stream.LeaveArray();
        }
        stream.EndArray();
        stream.ReadValue("Size", m_spriteRenderer.Size);
        stream.ReadValue("Anchor", m_spriteRenderer.Anchor);
    }

    void MeshRendererParser::Serialize(WriteStream& stream)
    {
        stream.BeginArray("Materials");
        for (auto& mat : m_meshRenderer.Materials) {
            stream.WriteArrayElement(mat != nullptr? mat->GetUUID().str(): "");
        }
        stream.EndArray();
    }

    void MeshRendererParser::Deserialize(ReadStream& stream)
    {
        size_t size = stream.BeginArray("Materials");
        for (size_t i = 0; i < size; ++i) {
            stream.EnterArray(i);
            std::string uuid;
            stream.ReadArrayElement(uuid);
            m_meshRenderer.Materials.push_back(ResourceManager::Instance().LoadResource<Material>(sole::rebuild(uuid)));
            stream.LeaveArray();
        }
        stream.EndArray();
    }

    void MeshFilterParser::Serialize(WriteStream& stream)
    {
        stream.WriteValue("Mesh", m_meshFilter.MeshPtr != nullptr? m_meshFilter.MeshPtr->GetUUID().str(): "");
    }

    void MeshFilterParser::Deserialize(ReadStream& stream)
    {
        std::string uuid;
        stream.ReadValue("Mesh", uuid);
        m_meshFilter.MeshPtr = ResourceManager::Instance().LoadResource<Mesh>(sole::rebuild(uuid));
    }

    void SceneObjectDelegateParser::Serialize(WriteStream& stream)
    {
        stream.WriteValue("IsVisible", m_sceneObjectDelegate.IsVisible);
        if (m_sceneObjectDelegate.GetParent()) {
            stream.WriteValue("Parent", m_sceneObjectDelegate.GetParent().GetComponent<Serializable>().UUID.str());
        }
    }

    void SceneObjectDelegateParser::Deserialize(ReadStream& stream)
    {
        stream.ReadValue("IsVisible", m_sceneObjectDelegate.IsVisible);
        if (stream.HasValue("Parent")) {
            std::string parentUUID;
            stream.ReadValue("Parent", parentUUID);
            sole::uuid uuid = sole::rebuild(parentUUID);
            const auto& sos = m_sceneObjectDelegate.This.GetScene()->GetSceneObjects();
            auto iter = std::find_if(sos.begin(), sos.end(), [&uuid](const SceneObject& so)->bool{
                return so.GetComponent<Serializable>().UUID == uuid;
            });
            if (iter != sos.end()) {
                iter->GetComponent<SceneObjectDelegate>().AddChild(m_sceneObjectDelegate.This);
            }
        }
    }
}
