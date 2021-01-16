//
//  ComponentParser.hpp
//  Lutra
//
//  Created by JasonCheng on 2021/1/11.
//

#ifndef ComponentParser_hpp
#define ComponentParser_hpp
#include "Parser.h"
#include <memory>
#include <limits>

namespace Lutra {

    enum class ComponentType
    {
        Tag = 0,
        Transform,
        SceneObjectDelegate,
        Camera,
        SpriteRenderer,
        MeshRenderer,
        MeshFilter,
        Serializable
    };

    class ComponentParser : public Parser
    {
    public:
        ComponentParser() = default;
        virtual ~ComponentParser() = default;
        
    };

    class Tag;
    class TagParser : public ComponentParser
    {
    public:
        
        TagParser(Tag& tag)
        : m_tag(tag)
        {}
        
        virtual void Serialize(WriteStream& stream) override;
        virtual void Deserialize(ReadStream& stream) override;
        
    private:
        
        Tag& m_tag;
    };

    class Transform;
    class TransformParser : public ComponentParser
    {
    public:
        
        TransformParser(Transform& transform)
        : m_transform(transform)
        {}
        
        virtual void Serialize(WriteStream& stream) override;
        virtual void Deserialize(ReadStream& stream) override;
        
    private:
        
        Transform& m_transform;
    };

    class Camera;
    class CameraParser : public ComponentParser
    {
    public:
        CameraParser(Camera& camera)
        : m_camera(camera)
        {}
        
        virtual void Serialize(WriteStream& stream) override;
        virtual void Deserialize(ReadStream& stream) override;
        
    private:
        
        Camera& m_camera;
    };

    class SpriteRenderer;
    class SpriteRendererParser : public ComponentParser
    {
    public:
        SpriteRendererParser(SpriteRenderer& sr)
        : m_spriteRenderer(sr)
        {}
        
        virtual void Serialize(WriteStream& stream) override;
        virtual void Deserialize(ReadStream& stream) override;
        
    private:
        
        SpriteRenderer& m_spriteRenderer;
    };

    class MeshRenderer;
    class MeshRendererParser : public ComponentParser
    {
    public:
        MeshRendererParser(MeshRenderer& mr)
        : m_meshRenderer(mr)
        {}
        
        virtual void Serialize(WriteStream& stream) override;
        virtual void Deserialize(ReadStream& stream) override;
        
    private:
        
        MeshRenderer& m_meshRenderer;
    };

    class MeshFilter;
    class MeshFilterParser : public ComponentParser
    {
    public:
        MeshFilterParser(MeshFilter& mf)
        : m_meshFilter(mf)
        {}
        
        virtual void Serialize(WriteStream& stream) override;
        virtual void Deserialize(ReadStream& stream) override;
        
    private:
        
        MeshFilter& m_meshFilter;
    };

    class SceneObjectDelegate;
    class Scene;
    class SceneObjectDelegateParser : public ComponentParser
    {
    public:
        SceneObjectDelegateParser(SceneObjectDelegate& sod)
        : m_sceneObjectDelegate(sod)
        {}
        
        virtual void Serialize(WriteStream& stream) override;
        virtual void Deserialize(ReadStream& stream) override;
        
    private:
        
        //uint32_t m_parentID = std::numeric_limits<uint32_t>::max();
        SceneObjectDelegate& m_sceneObjectDelegate;
    };
}

#endif /* ComponentParser_hpp */
