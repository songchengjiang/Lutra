//
//  MaterialBuilder.cpp
//  Editor
//
//  Created by JasonCheng on 2021/1/13.
//

#include "MaterialBuilder.h"

namespace LutraEditor {

    std::shared_ptr<Lutra::Material> MaterialBuilder::BuildUnlint()
    {
        const char* Vertex_Shader = R"(
        layout(location = 0) in vec3 a_position;
        layout(location = 1) in vec3 a_normal;
        layout(location = 2) in vec2 a_texcoord0;

        uniform mat4 u_ModelViewProjMat;
        uniform mat3 u_NormalWorldMat;

        out vec3 v_normal;
        out vec2 v_texcoord0;

        void main() {
            v_texcoord0 = a_texcoord0;
            v_normal = u_NormalWorldMat * a_normal;
            gl_Position = u_ModelViewProjMat * vec4(a_position, 1.0);
        }
        )";

        const char* Fragment_Shader = R"(
        uniform sampler2D u_BaseTex;

        in vec3 v_normal;
        in vec2 v_texcoord0;

        out vec4 FragColor;
        void main() {
            vec4 color = texture(u_BaseTex, v_texcoord0);
            FragColor = color;
        }
        )";
        auto material = Lutra::ResourceManager::Instance().CreateResource<Lutra::Material>();
        material->AddPass(std::shared_ptr<Lutra::Pass>(new Lutra::Pass({
            Vertex_Shader,
            Fragment_Shader,
            Lutra::BlendMode::Disabled,
            Lutra::CullMode::Back,
            true,
            true,
            true,
            glm::bvec4(true)
        })));
        material->GetPass(0)->GetShader().SetSampler("u_BaseTex", nullptr);
        return material;
    }

}
