//
//  BlueprintVM.hpp
//  Editor
//
//  Created by JasonCheng on 2021/1/19.
//

#ifndef BlueprintVM_hpp
#define BlueprintVM_hpp
#include <vector>
#include <functional>
#include "Varient.h"

namespace LutraEditor {

    class BlueprintVM
    {
    public:
        virtual ~BlueprintVM() = default;
        
        virtual void Compile() = 0;
    };

    struct FunctionVarient
    {
        std::string Name;
        Varient Value;
    };

    class Functions
    {
    public:
        
        virtual ~Functions() = default;
        virtual Functions& Assignment(const FunctionVarient& inValue, const FunctionVarient& outValue) = 0;
        
        virtual Functions& TextureFetch(const FunctionVarient& sampler, const FunctionVarient& texcoord, const FunctionVarient& outValue) = 0;
        
        virtual Functions& Abs(const FunctionVarient& inValue, const FunctionVarient& outValue) = 0;
        virtual Functions& Add(const FunctionVarient& inA, const FunctionVarient& inB, const FunctionVarient& outValue) = 0;
        virtual Functions& Ceil(const FunctionVarient& inValue, const FunctionVarient& outValue) = 0;
        virtual Functions& Clamp(const FunctionVarient& inA, const FunctionVarient& inMin, const FunctionVarient& inMax, const FunctionVarient& outValue) = 0;
        virtual Functions& Divide(const FunctionVarient& inA, const FunctionVarient& inB, const FunctionVarient& outValue) = 0;
        virtual Functions& DDX(const FunctionVarient& inValue, const FunctionVarient& outValue) = 0;
        virtual Functions& DDY(const FunctionVarient& inValue, const FunctionVarient& outValue) = 0;
        virtual Functions& Discard(const FunctionVarient& inA, const FunctionVarient& inBool, const FunctionVarient& outValue) = 0;
        virtual Functions& Floor(const FunctionVarient& inA, const FunctionVarient& outValue) = 0;
        virtual Functions& Fract(const FunctionVarient& inA, const FunctionVarient& outValue) = 0;
        virtual Functions& Max(const FunctionVarient& inA, const FunctionVarient& inB, const FunctionVarient& outValue) = 0;
        virtual Functions& Min(const FunctionVarient& inA, const FunctionVarient& inB, const FunctionVarient& outValue) = 0;
        virtual Functions& Mix(const FunctionVarient& inA, const FunctionVarient& inB, const FunctionVarient& inAlpha, const FunctionVarient& outValue) = 0;
        virtual Functions& Mod(const FunctionVarient& inA, const FunctionVarient& inDivider, const FunctionVarient& outValue) = 0;
        virtual Functions& Multiply(const FunctionVarient& inA, const FunctionVarient& inB, const FunctionVarient& outValue) = 0;
        virtual Functions& Negate(const FunctionVarient& inA, const FunctionVarient& outValue) = 0;
        virtual Functions& Round(const FunctionVarient& inA, const FunctionVarient& outValue) = 0;
        virtual Functions& Sign(const FunctionVarient& inA, const FunctionVarient& outValue) = 0;
        virtual Functions& SmoothStep(const FunctionVarient& inA, const FunctionVarient& inB, const FunctionVarient& inAlpha, const FunctionVarient& outValue) = 0;
        virtual Functions& Step(const FunctionVarient& inA, const FunctionVarient& inB, const FunctionVarient& outValue) = 0;
        virtual Functions& Subtract(const FunctionVarient& inA, const FunctionVarient& inB, const FunctionVarient& outValue) = 0;
        
        virtual Functions& ConstructVector(const FunctionVarient& inValueX, const FunctionVarient& inValueY, const FunctionVarient& inValueZ, const FunctionVarient& inValueW, const FunctionVarient& outValue) = 0;
        virtual Functions& SplitVector(const FunctionVarient& inValue, const FunctionVarient& outValueX, const FunctionVarient& outValueY, const FunctionVarient& outValueZ, const FunctionVarient& outValueW) = 0;
    };

    class FunctionsImpGL : public Functions
    {
    public:

        virtual Functions& Assignment(const FunctionVarient& inValue, const FunctionVarient& outValue) override;
        
        virtual Functions& TextureFetch(const FunctionVarient& sampler, const FunctionVarient& texcoord, const FunctionVarient& outValue) override;
        
        virtual Functions& Abs(const FunctionVarient& inValue, const FunctionVarient& outValue) override;
        virtual Functions& Add(const FunctionVarient& inA, const FunctionVarient& inB, const FunctionVarient& outValue) override;
        virtual Functions& Ceil(const FunctionVarient& inValue, const FunctionVarient& outValue) override;
        virtual Functions& Clamp(const FunctionVarient& inA, const FunctionVarient& inMin, const FunctionVarient& inMax, const FunctionVarient& outValue) override;
        virtual Functions& Divide(const FunctionVarient& inA, const FunctionVarient& inB, const FunctionVarient& outValue) override;
        virtual Functions& DDX(const FunctionVarient& inValue, const FunctionVarient& outValue) override;
        virtual Functions& DDY(const FunctionVarient& inValue, const FunctionVarient& outValue) override;
        virtual Functions& Discard(const FunctionVarient& inA, const FunctionVarient& inBool, const FunctionVarient& outValue) override;
        virtual Functions& Floor(const FunctionVarient& inA, const FunctionVarient& outValue) override;
        virtual Functions& Fract(const FunctionVarient& inA, const FunctionVarient& outValue) override;
        virtual Functions& Max(const FunctionVarient& inA, const FunctionVarient& inB, const FunctionVarient& outValue) override;
        virtual Functions& Min(const FunctionVarient& inA, const FunctionVarient& inB, const FunctionVarient& outValue) override;
        virtual Functions& Mix(const FunctionVarient& inA, const FunctionVarient& inB, const FunctionVarient& inAlpha, const FunctionVarient& outValue) override;
        virtual Functions& Mod(const FunctionVarient& inA, const FunctionVarient& inDivider, const FunctionVarient& outValue) override;
        virtual Functions& Multiply(const FunctionVarient& inA, const FunctionVarient& inB, const FunctionVarient& outValue) override;
        virtual Functions& Negate(const FunctionVarient& inA, const FunctionVarient& outValue) override;
        virtual Functions& Round(const FunctionVarient& inA, const FunctionVarient& outValue) override;
        virtual Functions& Sign(const FunctionVarient& inA, const FunctionVarient& outValue) override;
        virtual Functions& SmoothStep(const FunctionVarient& inA, const FunctionVarient& inB, const FunctionVarient& inAlpha, const FunctionVarient& outValue) override;
        virtual Functions& Step(const FunctionVarient& inA, const FunctionVarient& inB, const FunctionVarient& outValue) override;
        virtual Functions& Subtract(const FunctionVarient& inA, const FunctionVarient& inB, const FunctionVarient& outValue) override;
        
        virtual Functions& ConstructVector(const FunctionVarient& inValueX, const FunctionVarient& inValueY, const FunctionVarient& inValueZ, const FunctionVarient& inValueW, const FunctionVarient& outValue) override;
        virtual Functions& SplitVector(const FunctionVarient& inValue, const FunctionVarient& outValueX, const FunctionVarient& outValueY, const FunctionVarient& outValueZ, const FunctionVarient& outValueW) override;
    
    private:
        
        void insertValue(const FunctionVarient& var);
        
    private:
        
        std::vector<FunctionVarient> m_varients;
        std::string m_functions;
        friend class MaterialGLVM;
    };

    class MaterialVM : public BlueprintVM
    {
    public:
        
        MaterialVM() = default;
        virtual ~MaterialVM() = default;
        
        void SetUniform(const FunctionVarient& var);
        
        Functions* GetVertexFunctions() { return m_vertexFuns.get(); }
        Functions* GetFragmentFunctions() { return m_fragmentFuns.get(); }
        
    protected:
        
        std::vector<FunctionVarient> m_uniforms;
        std::unique_ptr<Functions>   m_vertexFuns;
        std::unique_ptr<Functions>   m_fragmentFuns;
    };

    class MaterialGLVM : public MaterialVM
    {
    public:
        
        MaterialGLVM(const std::shared_ptr<Lutra::Material>& material);
        virtual ~MaterialGLVM() = default;
        
        virtual void Compile() override;
        
    private:
        
        bool CheckVarientAvailable(const FunctionVarient& var);
        
    private:
        
        std::shared_ptr<Lutra::Material> m_material;
    };

}

#endif /* BlueprintVM_hpp */
