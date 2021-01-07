//
//  Pass.hpp
//  Lutra
//
//  Created by JasonCheng on 2021/1/4.
//

#ifndef Pass_hpp
#define Pass_hpp
#include "Scene/Shader.h"

namespace Lutra {

    enum class BlendMode
    {
        Disabled,
        Normal,
        Screen,
        PremultipliedAlpha,
        Add,
        Multiply
    };

    enum class CullMode
    {
        Front,
        Back,
        FrontAndBack
    };

    struct PassBuilder
    {
        std::string VertexShader;
        std::string FragmentShader;
        BlendMode   BdMode;
        CullMode    ClMode;
        bool        DepthTest;
        bool        DepthWrite;
        bool        TwoSided;
        glm::bvec4  ColorMask;
    };

    class Pass
    {
    public:
        Pass(const PassBuilder& builder);
        ~Pass() = default;
        
        Pass(const Pass& other) = default;
        
        BlendMode& GetBlendMode() { return m_blendMode; }
        CullMode& GetCullMode() { return m_cullMode; }
        bool& GetDepthTest() { return m_depthTest; }
        bool& GetDepthWrite() { return m_depthWrite; }
        bool& GetTwoSided() { return m_twoSided; }
        glm::bvec4& GetColorMask() { return m_colorMask; }
        
        Shader& GetShader() { return m_shader; }
        const Shader& GetShader() const { return m_shader; }
        
        Pass& operator=(const Pass& other) = default;
        
    private:
        
        BlendMode  m_blendMode;
        CullMode   m_cullMode;
        bool       m_depthTest;
        bool       m_depthWrite;
        bool       m_twoSided;
        glm::bvec4 m_colorMask;
        
        Shader m_shader;
    };
}

#endif /* Pass_hpp */
