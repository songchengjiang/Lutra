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
#include "Buffer.h"
#include "FrameBuffer.h"
#include <gtc/type_ptr.hpp>

#include "RenderDevice_GL.h"

namespace Lutra {

    static float CaculateUsageRate(uint32_t usage)
    {
        usage = (usage & 0x55555555) + ((usage >> 1) & 0x55555555);
        usage = (usage & 0x33333333) + ((usage >> 2) & 0x33333333);
        usage = (usage & 0x0f0f0f0f) + ((usage >> 4) & 0x0f0f0f0f);
        usage = (usage & 0x00ff00ff) + ((usage >> 8) & 0x00ff00ff);
        usage = (usage & 0x0000ffff) + ((usage >> 16) & 0x0000ffff);

        return (float)usage / (float)32;
    }

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

    void Graphic::SetRenderTexture(const std::shared_ptr<RenderTexture>& renderTexture, bool binding)
    {
        auto hash = std::hash<RenderTexture*>()(renderTexture.get());
        std::shared_ptr<FrameBuffer> fbo;
        auto iter = m_frameBufferList.find(hash);
        if (iter == m_frameBufferList.end()) {
            FramebufferSpecification spec;
            spec.ColorAttachments.push_back(getDeviceTextureBy(renderTexture.get()));
            spec.DepthStencilAttachment = m_renderDevice->CreateTexture2D(renderTexture->GetWidth(), renderTexture->GetHeight(), (DeviceTextureForamt)renderTexture->GetDepthFormat());
            fbo = m_renderDevice->CreateFrameBuffer(spec);
            m_frameBufferList.insert(std::make_pair(hash, fbo));
        }else {
            fbo = iter->second;
        }
        
//        if (renderTarget->GetWidth() != fbo->GetSpecification().Width
//         || renderTarget->GetHeight() != fbo->GetSpecification().Height) {
//            fbo->Resize();
//        }
        if (binding)
            fbo->Bind();
        else
            fbo->Unbind();
    }

    std::shared_ptr<VertexArray> Graphic::getVertexArray(Mesh* mesh, uint8_t submesh)
    {
        auto vboHash = std::hash<Mesh*>()(mesh);
        std::shared_ptr<VertexBuffer> VBO;
        auto vboIter = m_vboList.find(vboHash);
        if (vboIter == m_vboList.end()) {
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
            
            if (!mesh->Colors.empty()) {
                bufferElements.push_back({BufferDataType::Float4, "a_color", false});
            }
            
            if (!mesh->Texcoord0.empty()) {
                bufferElements.push_back({BufferDataType::Float2, "a_texcoord0", false});
            }
            
            if (!mesh->Texcoord1.empty()) {
                bufferElements.push_back({BufferDataType::Float2, "a_texcoord1", false});
            }
            VBO = m_renderDevice->CreateVertexBuffer(mesh->SequentialBuffer.data(), (uint32_t)mesh->SequentialBuffer.size(), bufferElements);
            m_vboList.insert(std::make_pair(vboHash, VBO));
            mesh->MarkForUpdate = false;
        }else {
            VBO = vboIter->second;
            if (mesh->MarkForUpdate) {
                VBO->SetData(mesh->SequentialBuffer.data(), (uint32_t)mesh->SequentialBuffer.size());
                mesh->MarkForUpdate = false;
            }
        }
        
        auto vaoHash = vboHash + std::hash<uint8_t>()(submesh);
        std::shared_ptr<VertexArray> VAO;
        auto meshIter = m_vaoList.find(vaoHash);
        if (meshIter == m_vaoList.end()) {
            std::vector<std::shared_ptr<VertexBuffer>> vertexBuffers;
            vertexBuffers.push_back(VBO);
            std::shared_ptr<IndexBuffer> indexBuffer;
            if (!mesh->SubMeshList[submesh].Indices.empty()) {
                indexBuffer = m_renderDevice->CreateIndexBuffer(mesh->SubMeshList[submesh].Indices.data(), (uint32_t)mesh->SubMeshList[submesh].Indices.size());
            }
            VAO = m_renderDevice->CreateVertexArray(vertexBuffers, indexBuffer);
            m_vaoList.insert(std::make_pair(vaoHash, VAO));
        }else {
            VAO = meshIter->second;
        }
        return VAO;
    }

    std::shared_ptr<Program> Graphic::getProgram(Material* material, uint32_t passID)
    {
        auto hash = material->GetPass(passID)->GetShader().GetHash();
        std::shared_ptr<Program> program;
        auto materialIter = m_programList.find(hash);
        if (materialIter == m_programList.end()) {
            auto& pass = material->GetPass(0);
            program = m_renderDevice->CreateProgram(pass->GetShader().GetVertexSource(), pass->GetShader().GetFragmentSource());
            m_programList.insert(std::make_pair(hash, program));
        }else {
            program = materialIter->second;
        }
        return program;
    }

    std::shared_ptr<DeviceTexture> Graphic::getDeviceTextureBy(Texture* tex)
    {
        auto hash = std::hash<Texture*>()(tex);
        std::shared_ptr<DeviceTexture> deviceTexture;
        auto iter = m_deviceTextureList.find(hash);
        if (iter == m_deviceTextureList.end()) {
            if (tex->GetType() == TextureType::TEX2D) {
                auto tex2D = static_cast<Texture2D*>(tex);
                deviceTexture = m_renderDevice->CreateTexture2D(tex2D->GetWidth(), tex2D->GetHeight(), (DeviceTextureForamt)tex2D->GetFormat());
                if (!tex2D->GetData().empty())
                    deviceTexture->SetData(tex2D->GetData().data(), (uint32_t)tex2D->GetData().size());
            }else if (tex->GetType() == TextureType::RENDER) {
                auto renderTex = static_cast<RenderTexture*>(tex);
                deviceTexture = m_renderDevice->CreateTexture2D(renderTex->GetWidth(), renderTex->GetHeight(), (DeviceTextureForamt)renderTex->GetColorFormat());
            }
            tex->SetTextureID(deviceTexture->GetID());
            m_deviceTextureList.insert(std::make_pair(hash, deviceTexture));
        }else {
            deviceTexture = iter->second;
        }
        
        if (deviceTexture->GetFilter() != (DeviceTextureFilter)tex->GetFilter()) {
            deviceTexture->SetFilter((DeviceTextureFilter)tex->GetFilter());
        }
        if (deviceTexture->GettWrap() != (DeviceTextureWrap)tex->GetWrap()) {
            deviceTexture->SetWrap((DeviceTextureWrap)tex->GetWrap());
        }
        return deviceTexture;
    }

    void Graphic::DrawMesh(Mesh* mesh, Material* material, Camera* camera, uint8_t submesh, uint8_t passID, const glm::mat4& modelMatrix)
    {
        
        RenderQueueType queueType = material->GetPass(passID)->GetBlendMode() == BlendMode::Disabled? RenderQueueType::Opacity: RenderQueueType::Transparent;
        
        glm::vec4 distanceToCamera = camera->ViewMat * modelMatrix * glm::vec4(mesh->SubMeshList[submesh].BBox.Center(), 1.0f);
        if (queueType == RenderQueueType::Opacity) {
            distanceToCamera = -distanceToCamera;
        }
        m_renderQueue[queueType].push_back({mesh, material, camera, submesh, passID, modelMatrix, distanceToCamera.z / distanceToCamera.w});
    }

    void Graphic::drawCommand(const RenderCommand& cmd)
    {
        auto VAO = getVertexArray(cmd.Mesh_, cmd.SubMesh);
        auto program = getProgram(cmd.Material_, cmd.Pass);
        
        VAO->Bind(program);
        
        uint32_t texUnit = 0;
        for (auto &value : cmd.Material_->GetPass(0)->GetShader().GetValues()) {
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
                    if (*value.second.Value_.tex != nullptr)
                        program->SetSampler(value.first, getDeviceTextureBy(value.second.Value_.tex->get()), texUnit++);
                    break;
                    
                default:
                    break;
            }
        }
        
        glm::mat4 mv = cmd.Camera_->ViewMat * cmd.ModelMatrix;
        glm::mat4 mvp = cmd.Camera_->ProjectMat * mv;
        program->SetUniform("u_ModelViewProjMat", &mvp, 1);
        
        program->SetUniform("u_ModelViewMat", &mv, 1);
        
        glm::mat3 normalWorld{cmd.ModelMatrix};
        normalWorld = glm::transpose(glm::inverse(normalWorld));
        program->SetUniform("u_NormalWorldMat", &normalWorld, 1);
        
        glm::mat3 normal{mv};
        normal = glm::transpose(glm::inverse(normal));
        program->SetUniform("u_NormalMat", &normal, 1);
        
        auto& pass = cmd.Material_->GetPass(cmd.Pass);
        m_renderDevice->DrawIndexed({(GraphicBlendMode)pass->GetBlendMode(),
                                          (GraphicCullMode)pass->GetCullMode(),
                                          pass->GetDepthTest(),
                                          pass->GetDepthWrite(),
                                          pass->GetTwoSided(),
                                          pass->GetColorMask()},
                                         (DevicePrimitiveType)cmd.Mesh_->SubMeshList[cmd.SubMesh].Type,
                                          VAO->GetIndexBuffer()->GetSize());
        VAO->Unbind();
    }

    void Graphic::Finish()
    {
        for (auto& renderQueue : m_renderQueue) {
            std::sort(renderQueue.second.begin(), renderQueue.second.end(), [](const RenderCommand& left, const RenderCommand& right){
                return left.SortKey < right.SortKey;
            });
            for (auto& cmd : renderQueue.second) {
                drawCommand(cmd);
            }
        }
        m_renderQueue.clear();
    }

    void Graphic::Begin()
    {
        
    }

    template<typename T>
    void ResourceRecovery(std::unordered_map<std::size_t, std::shared_ptr<T>>& resList)
    {
        for (auto iter = resList.begin(); iter != resList.end();) {
            iter->second->Update();
            if (CaculateUsageRate(iter->second->GetUsage()) == 0) {
                iter = resList.erase(iter);
            }else {
                ++iter;
            }
        }
    }

    void Graphic::End()
    {
        ResourceRecovery(m_vboList);
        ResourceRecovery(m_vaoList);
        ResourceRecovery(m_programList);
        ResourceRecovery(m_deviceTextureList);
        ResourceRecovery(m_frameBufferList);
        
        m_renderDevice->SetGraphicState({GraphicBlendMode::Disabled, GraphicCullMode::Back, true, true, true, glm::bvec4(true)});
    }

}
