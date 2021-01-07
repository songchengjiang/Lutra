//
//  Graphic.cpp
//  Lutra
//
//  Created by JasonCheng on 2021/1/4.
//

#include "Graphic.h"
#include "Mesh.h"
#include "Material.h"
#include "Pass.h"
#include "Shader.h"
#include "Camera.h"
#include "Texture.h"
#include "DeviceTexture.h"
#include "Program.h"
#include "VertexArray.h"
#include "FrameBuffer.h"
#include "RenderTarget.h"
#include <gtc/type_ptr.hpp>

#include "RenderDevice_GL.h"

namespace Lutra {

    Graphic::Graphic(API api)
    {
        switch (api) {
            case API::OpenGL: m_renderDevice = std::unique_ptr<RenderDevice>(new RenderDeviceGL); break;
            default: break;
        }
    }

    void Graphic::SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
    {
        m_renderDevice->SetViewport(x, y, width, height);
    }

    void Graphic::SetClearColor(const glm::vec4& color)
    {
        m_renderDevice->SetClearColor(color);
    }

    void Graphic::Clear(uint8_t flag)
    {
        m_renderDevice->Clear(flag);
    }

    void Graphic::SetRenderTarget(std::shared_ptr<RenderTarget>& renderTarget)
    {
        std::shared_ptr<FrameBuffer> fbo;
        auto iter = m_renderTargetList.find(renderTarget.get());
        if (iter == m_renderTargetList.end()) {
            FramebufferSpecification spec;
            spec.Width = renderTarget->GetWidth();
            spec.Height = renderTarget->GetHeight();
            if (renderTarget->GetType() == RenderTargetType::Texture) {
                auto renderTexture = static_cast<RenderTexture*>(renderTarget.get());
                for (auto& colorAtt : renderTexture->ColorAttachments) {
                    spec.ColorAttachments.push_back(getDeviceTextureBy(colorAtt.get()));
                }
                if (renderTexture->DepthStencilAttachment != nullptr) {
                    spec.DepthStencilAttachment = getDeviceTextureBy(renderTexture->DepthStencilAttachment.get());
                }
            }
            fbo = m_renderDevice->CreateFrameBuffer(spec);
        }else {
            fbo = iter->second;
        }
        
        if (renderTarget->GetWidth() != fbo->GetSpecification().Width
         || renderTarget->GetHeight() != fbo->GetSpecification().Height) {
            fbo->Resize(renderTarget->GetWidth(), renderTarget->GetHeight());
        }
        
        fbo->Bind();
    }

    std::shared_ptr<VertexArray> Graphic::updateVertexArray(Mesh* mesh, uint8_t submesh)
    {
        std::shared_ptr<VertexArray> VAO;
        auto meshIter = m_meshList.find(std::make_pair(mesh, submesh));
        if (meshIter == m_meshList.end()) {
            std::vector<std::shared_ptr<VertexBuffer>> vertexBuffers;
            
            std::vector<BufferElement> bufferElements;
            if (!mesh->Vertices.empty()) {
                bufferElements.push_back({BufferDataType::Float3, "a_position", false});
            }
            
            if (!mesh->Normals.empty()) {
                bufferElements.push_back({BufferDataType::Float3, "a_normal", false});
            }
            
            if (!mesh->Tangents.empty()) {
                bufferElements.push_back({BufferDataType::Float3, "a_tangent", false});
            }
            
            if (!mesh->Texcoord0.empty()) {
                bufferElements.push_back({BufferDataType::Float2, "a_texcoord0", false});
            }
            
            if (!mesh->Texcoord1.empty()) {
                bufferElements.push_back({BufferDataType::Float2, "a_texcoord1", false});
            }
            auto vb = m_renderDevice->CreateVertexBuffer(mesh->SequentialBuffer.data(), (uint32_t)mesh->SequentialBuffer.size(), bufferElements);
            vertexBuffers.push_back(vb);
            
            std::shared_ptr<IndexBuffer> indexBuffer;
            if (!mesh->SubMeshList[submesh].Indices.empty()) {
                indexBuffer = m_renderDevice->CreateIndexBuffer(mesh->SubMeshList[submesh].Indices.data(), (uint32_t)mesh->SubMeshList[submesh].Indices.size());
            }
            VAO = m_renderDevice->CreateVertexArray(vertexBuffers, indexBuffer);
            m_meshList.insert(std::make_pair(std::make_pair(mesh, submesh), VAO));
            mesh->MarkForUpdate = false;
        }else {
            VAO = meshIter->second;
            if (meshIter->first.first->MarkForUpdate) {
                auto& vbos = VAO->GetVertexBuffers();
                vbos.front()->SetData(mesh->SequentialBuffer.data(), (uint32_t)mesh->SequentialBuffer.size());
                meshIter->first.first->MarkForUpdate = false;
            }
        }
        return VAO;
    }

    std::shared_ptr<Program> Graphic::updateProgram(Material* material, uint32_t passID)
    {
        std::shared_ptr<Program> program;
        auto materialIter = m_materialList.find(std::make_pair(material, passID));
        if (materialIter == m_materialList.end()) {
            auto& pass = material->GetPass(0);
            program = m_renderDevice->CreateProgram(pass.GetShader().GetVertexSource(), pass.GetShader().GetFragmentSource());
            m_materialList.insert(std::make_pair(std::make_pair(material, 0), program));
        }else {
            program = materialIter->second;
        }
        return program;
    }

    std::shared_ptr<DeviceTexture> Graphic::getDeviceTextureBy(Texture* tex)
    {
        std::shared_ptr<DeviceTexture> deviceTexture;
        auto iter = m_textureList.find(tex);
        if (iter == m_textureList.end()) {
            if (tex->GetType() == TextureType::TEX2D) {
                auto tex2D = static_cast<Texture2D*>(tex);
                deviceTexture = m_renderDevice->CreateTexture2D(tex2D->GetWidth(), tex2D->GetHeight(), (DeviceTextureForamt)tex2D->GetFormat());
                if (!tex2D->GetData().empty())
                    deviceTexture->SetData(tex2D->GetData().data(), (uint32_t)tex2D->GetData().size());
                m_textureList.insert(std::make_pair(tex, deviceTexture));
            }
        }else {
            deviceTexture = iter->second;
        }
        return deviceTexture;
    }

    void Graphic::DrawMesh(Mesh* mesh, Material* material, Camera* camera, const glm::mat4& modelMatrix, uint8_t submesh)
    {
        uint32_t passID = 0;
        auto VAO = updateVertexArray(mesh, submesh);
        auto program = updateProgram(material, passID);
        
        program->Bind();
        
        uint32_t texUnit = 0;
        for (auto &value : material->GetPass(0).GetShader().GetValues()) {
            switch (value.second.Type_) {
                case ShaderValue::Type::Float:
                    program->SetUniform(value.first, &value.second.Value_.v1, 1);
                    break;
                case ShaderValue::Type::Float2:
                    program->SetUniform(value.first, &value.second.Value_.v2, 1);
                    break;
                case ShaderValue::Type::Float3:
                    program->SetUniform(value.first, &value.second.Value_.v3, 1);
                    break;
                case ShaderValue::Type::Float4:
                    program->SetUniform(value.first, &value.second.Value_.v4, 1);
                    break;
                case ShaderValue::Type::Mat3:
                    program->SetUniform(value.first, value.second.Value_.m3, 1);
                    break;
                case ShaderValue::Type::Mat4:
                    program->SetUniform(value.first, value.second.Value_.m4, 1);
                    break;
                case ShaderValue::Type::Sampler:
                    program->SetSampler(value.first, getDeviceTextureBy(value.second.Value_.tex->get()), texUnit++);
                    break;
                    
                default:
                    break;
            }
        }
        
        glm::mat4 mv = camera->ViewMat * modelMatrix;
        glm::mat4 mvp = camera->ProjectMat * mv;
        program->SetUniform("u_ModelViewProjMat", &mvp, 1);
        
        glm::mat3 normalWorld{modelMatrix};
        normalWorld = glm::transpose(glm::inverse(normalWorld));
        program->SetUniform("u_NormalWorldMat", &normalWorld, 1);
        
        m_renderDevice->DrawIndexed(VAO);
    }

    void Graphic::Begin()
    {
        
    }

    void Graphic::End()
    {
        
    }

}
