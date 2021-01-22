//
//  MaterialNode.hpp
//  Editor
//
//  Created by JasonCheng on 2021/1/19.
//

#ifndef MaterialNode_hpp
#define MaterialNode_hpp
#include "BlueprintNode.h"

namespace LutraEditor {

    struct UnlitMaterialNode : BlueprintNode
    {
        virtual ~UnlitMaterialNode() = default;
        
        static std::string Type() { return "Shader"; }
        static std::string Group() { return "Shading Model"; }
        
        virtual void OnInit(BlueprintVM* vm) override;
        virtual bool Compile(BlueprintVM* vm) override;
    };

    struct ConstructVectorNode : BlueprintNode
    {
        virtual ~ConstructVectorNode() = default;
        
        static std::string Type() { return "Construct Vector"; }
        static std::string Group() { return "Channels"; }
        
        virtual void OnInit(BlueprintVM* vm) override;
        virtual bool Compile(BlueprintVM* vm) override;
    };

    struct SplitVectorNode : BlueprintNode
    {
        virtual ~SplitVectorNode() = default;
        
        static std::string Type() { return "Split Vector"; }
        static std::string Group() { return "Channels"; }
        
        virtual void OnInit(BlueprintVM* vm) override;
        virtual bool Compile(BlueprintVM* vm) override;
    };

    struct BoolParameterNode : BlueprintNode
    {
        virtual ~BoolParameterNode() = default;
        
        static std::string Type() { return "Bool Parameter"; }
        static std::string Group() { return "Parameters"; }
        
        virtual bool OnGUI() override;
        virtual void OnInit(BlueprintVM* vm) override;
        virtual bool Compile(BlueprintVM* vm) override;
        
    private:
        
        bool m_value{true};
    };

    struct IntParameterNode : BlueprintNode
    {
        virtual ~IntParameterNode() = default;
        
        static std::string Type() { return "Int Parameter"; }
        static std::string Group() { return "Parameters"; }
        
        virtual bool OnGUI() override;
        virtual void OnInit(BlueprintVM* vm) override;
        virtual bool Compile(BlueprintVM* vm) override;
        
    private:
        
        int m_value{0};
    };

    struct FloatParameterNode : BlueprintNode
    {
        virtual ~FloatParameterNode() = default;
        
        static std::string Type() { return "Float Parameter"; }
        static std::string Group() { return "Parameters"; }
        
        virtual bool OnGUI() override;
        virtual void OnInit(BlueprintVM* vm) override;
        virtual bool Compile(BlueprintVM* vm) override;
        
    private:
        
        float m_value{0.0f};
    };

    struct Float2ParameterNode : BlueprintNode
    {
        virtual ~Float2ParameterNode() = default;
        
        static std::string Type() { return "Float2 Parameter"; }
        static std::string Group() { return "Parameters"; }
        
        virtual bool OnGUI() override;
        virtual void OnInit(BlueprintVM* vm) override;
        virtual bool Compile(BlueprintVM* vm) override;
        
    private:
        
        glm::vec2 m_value{0.0f};
    };

    struct Float3ParameterNode : BlueprintNode
    {
        virtual ~Float3ParameterNode() = default;
        
        static std::string Type() { return "Float3 Parameter"; }
        static std::string Group() { return "Parameters"; }
        
        virtual bool OnGUI() override;
        virtual void OnInit(BlueprintVM* vm) override;
        virtual bool Compile(BlueprintVM* vm) override;
        
    private:
        
        glm::vec3 m_value{0.0f};
    };

    struct Float4ParameterNode : BlueprintNode
    {
        virtual ~Float4ParameterNode() = default;
        
        static std::string Type() { return "Float4 Parameter"; }
        static std::string Group() { return "Parameters"; }
        
        virtual bool OnGUI() override;
        virtual void OnInit(BlueprintVM* vm) override;
        virtual bool Compile(BlueprintVM* vm) override;
        
    private:
        
        glm::vec4 m_value{0.0f};
    };

    class ResourceBrowserWidget;
    struct TextureParameterNode : BlueprintNode
    {
        virtual ~TextureParameterNode() = default;
        
        static std::string Type() { return "Texture Parameter"; }
        static std::string Group() { return "Parameters"; }
        
        virtual bool OnGUI() override;
        virtual void OnInit(BlueprintVM* vm) override;
        virtual bool Compile(BlueprintVM* vm) override;
        
    private:
        
        std::shared_ptr<Lutra::Texture> m_value;
        std::shared_ptr<ResourceBrowserWidget> m_browserWidget;
    };

    struct SurfacePositionNode : BlueprintNode
    {
        virtual ~SurfacePositionNode() = default;
        
        static std::string Type() { return "Surface Position"; }
        static std::string Group() { return "Vertex"; }
        
        virtual void OnInit(BlueprintVM* vm) override;
        virtual bool Compile(BlueprintVM* vm) override;
        
    };

    struct SurfaceNormalNode : BlueprintNode
    {
        virtual ~SurfaceNormalNode() = default;
        
        static std::string Type() { return "Surface Normal"; }
        static std::string Group() { return "Vertex"; }
        
        virtual void OnInit(BlueprintVM* vm) override;
        virtual bool Compile(BlueprintVM* vm) override;
        
    };

    struct SurfaceTangentNode : BlueprintNode
    {
        virtual ~SurfaceTangentNode() = default;
        
        static std::string Type() { return "Surface Tangent"; }
        static std::string Group() { return "Vertex"; }
        
        virtual void OnInit(BlueprintVM* vm) override;
        virtual bool Compile(BlueprintVM* vm) override;
        
    };

    struct SurfaceColorNode : BlueprintNode
    {
        virtual ~SurfaceColorNode() = default;
        
        static std::string Type() { return "Surface Color"; }
        static std::string Group() { return "Vertex"; }
        
        virtual void OnInit(BlueprintVM* vm) override;
        virtual bool Compile(BlueprintVM* vm) override;
        
    };

    struct SurfaceTexcoord0Node : BlueprintNode
    {
        virtual ~SurfaceTexcoord0Node() = default;
        
        static std::string Type() { return "Surface Texcoord0"; }
        static std::string Group() { return "Vertex"; }
        
        virtual void OnInit(BlueprintVM* vm) override;
        virtual bool Compile(BlueprintVM* vm) override;
        
    };

    struct SurfaceTexcoord1Node : BlueprintNode
    {
        virtual ~SurfaceTexcoord1Node() = default;
        
        static std::string Type() { return "Surface Texcoord1"; }
        static std::string Group() { return "Vertex"; }
        
        virtual void OnInit(BlueprintVM* vm) override;
        virtual bool Compile(BlueprintVM* vm) override;
        
    };

    struct AbsNode : BlueprintNode
    {
        static std::string Type() { return "Abs"; }
        static std::string Group() { return "Math"; }
        
        virtual void OnInit(BlueprintVM* vm) override;
        virtual bool Compile(BlueprintVM* vm) override;
        
    };

    struct AddNode : BlueprintNode
    {
        static std::string Type() { return "Add"; }
        static std::string Group() { return "Math"; }
        
        virtual void OnInit(BlueprintVM* vm) override;
        virtual bool Compile(BlueprintVM* vm) override;
        
    };

    struct CeilNode : BlueprintNode
    {
        static std::string Type() { return "Ceil"; }
        static std::string Group() { return "Math"; }
        
        virtual void OnInit(BlueprintVM* vm) override;
        virtual bool Compile(BlueprintVM* vm) override;
        
    };

    struct ClampNode : BlueprintNode
    {
        static std::string Type() { return "Clamp"; }
        static std::string Group() { return "Math"; }
        
        virtual void OnInit(BlueprintVM* vm) override;
        virtual bool Compile(BlueprintVM* vm) override;
        
    };

    struct DivideNode : BlueprintNode
    {
        static std::string Type() { return "Divide"; }
        static std::string Group() { return "Math"; }
        
        virtual void OnInit(BlueprintVM* vm) override;
        virtual bool Compile(BlueprintVM* vm) override;
        
    };

    struct DDXNode : BlueprintNode
    {
        static std::string Type() { return "DDX"; }
        static std::string Group() { return "Math"; }
        
        virtual void OnInit(BlueprintVM* vm) override;
        virtual bool Compile(BlueprintVM* vm) override;
        
    };

    struct DDYNode : BlueprintNode
    {
        static std::string Type() { return "DDY"; }
        static std::string Group() { return "Math"; }
        
        virtual void OnInit(BlueprintVM* vm) override;
        virtual bool Compile(BlueprintVM* vm) override;
        
    };

    struct DiscardNode : BlueprintNode
    {
        static std::string Type() { return "Discard"; }
        static std::string Group() { return "Math"; }
        
        virtual void OnInit(BlueprintVM* vm) override;
        virtual bool Compile(BlueprintVM* vm) override;
        
    };

    struct FloorNode : BlueprintNode
    {
        static std::string Type() { return "Floor"; }
        static std::string Group() { return "Math"; }
        
        virtual void OnInit(BlueprintVM* vm) override;
        virtual bool Compile(BlueprintVM* vm) override;
        
    };

    struct FractNode : BlueprintNode
    {
        static std::string Type() { return "Fract"; }
        static std::string Group() { return "Math"; }
        
        virtual void OnInit(BlueprintVM* vm) override;
        virtual bool Compile(BlueprintVM* vm) override;
        
    };

    struct ReciprocalNode : BlueprintNode
    {
        static std::string Type() { return "Reciprocal"; }
        static std::string Group() { return "Math"; }
        
        virtual void OnInit(BlueprintVM* vm) override;
        virtual bool Compile(BlueprintVM* vm) override;
        
    };

    struct MaxNode : BlueprintNode
    {
        static std::string Type() { return "Max"; }
        static std::string Group() { return "Math"; }
        
        virtual void OnInit(BlueprintVM* vm) override;
        virtual bool Compile(BlueprintVM* vm) override;
        
    };

    struct MinNode : BlueprintNode
    {
        static std::string Type() { return "Min"; }
        static std::string Group() { return "Math"; }
        
        virtual void OnInit(BlueprintVM* vm) override;
        virtual bool Compile(BlueprintVM* vm) override;
        
    };

    struct MixNode : BlueprintNode
    {
        static std::string Type() { return "Mix"; }
        static std::string Group() { return "Math"; }
        
        virtual void OnInit(BlueprintVM* vm) override;
        virtual bool Compile(BlueprintVM* vm) override;
        
    };

    struct ModNode : BlueprintNode
    {
        static std::string Type() { return "Mod"; }
        static std::string Group() { return "Math"; }
        
        virtual void OnInit(BlueprintVM* vm) override;
        virtual bool Compile(BlueprintVM* vm) override;
        
    };

    struct MultiplyNode : BlueprintNode
    {
        static std::string Type() { return "Multiply"; }
        static std::string Group() { return "Math"; }
        
        virtual void OnInit(BlueprintVM* vm) override;
        virtual bool Compile(BlueprintVM* vm) override;
        
    };

    struct NegateNode : BlueprintNode
    {
        static std::string Type() { return "Negate"; }
        static std::string Group() { return "Math"; }
        
        virtual void OnInit(BlueprintVM* vm) override;
        virtual bool Compile(BlueprintVM* vm) override;
        
    };

    struct OneMinusNode : BlueprintNode
    {
        static std::string Type() { return "One Minus"; }
        static std::string Group() { return "Math"; }
        
        virtual void OnInit(BlueprintVM* vm) override;
        virtual bool Compile(BlueprintVM* vm) override;
        
    };

    struct RoundNode : BlueprintNode
    {
        static std::string Type() { return "Round"; }
        static std::string Group() { return "Math"; }
        
        virtual void OnInit(BlueprintVM* vm) override;
        virtual bool Compile(BlueprintVM* vm) override;
        
    };

    struct ScaleAndOffsetNode : BlueprintNode
    {
        static std::string Type() { return "Scale And Offset"; }
        static std::string Group() { return "Math"; }
        
        virtual void OnInit(BlueprintVM* vm) override;
        virtual bool Compile(BlueprintVM* vm) override;
        
    };

    struct SignNode : BlueprintNode
    {
        static std::string Type() { return "Sign"; }
        static std::string Group() { return "Math"; }
        
        virtual void OnInit(BlueprintVM* vm) override;
        virtual bool Compile(BlueprintVM* vm) override;
        
    };

    struct SmoothStepNode : BlueprintNode
    {
        static std::string Type() { return "SmoothStep"; }
        static std::string Group() { return "Math"; }
        
        virtual void OnInit(BlueprintVM* vm) override;
        virtual bool Compile(BlueprintVM* vm) override;
        
    };

    struct StepNode : BlueprintNode
    {
        static std::string Type() { return "Step"; }
        static std::string Group() { return "Math"; }
        
        virtual void OnInit(BlueprintVM* vm) override;
        virtual bool Compile(BlueprintVM* vm) override;
        
    };

    struct SubtractNode : BlueprintNode
    {
        static std::string Type() { return "Subtract"; }
        static std::string Group() { return "Math"; }
        
        virtual void OnInit(BlueprintVM* vm) override;
        virtual bool Compile(BlueprintVM* vm) override;
        
    };

    struct ScaleCoordsNode : BlueprintNode
    {
        static std::string Type() { return "Scale Coords"; }
        static std::string Group() { return "Common"; }
        
        virtual void OnInit(BlueprintVM* vm) override;
        virtual bool Compile(BlueprintVM* vm) override;
        
    };

    struct DeltaTimeNode : BlueprintNode
    {
        static std::string Type() { return "Delta Time"; }
        static std::string Group() { return "Engine"; }
        
        virtual void OnInit(BlueprintVM* vm) override;
        virtual bool Compile(BlueprintVM* vm) override;
        
    };

    struct ElapsedTimeNode : BlueprintNode
    {
        static std::string Type() { return "Elapsed Time"; }
        static std::string Group() { return "Engine"; }
        
        virtual void OnInit(BlueprintVM* vm) override;
        virtual bool Compile(BlueprintVM* vm) override;
        
    };

}

#endif /* MaterialNode_hpp */
