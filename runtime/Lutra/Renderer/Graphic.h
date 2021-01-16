//
//  Graphic.hpp
//  Lutra
//
//  Created by JasonCheng on 2021/1/4.
//

#ifndef Graphic_hpp
#define Graphic_hpp
#include <glm.hpp>
#include <memory>
#include <unordered_map>
#include <map>
#include <functional>
#include "RenderDevice.h"

namespace Lutra {

    struct Mesh;
    struct SubMesh;
    class Material;
    class Shader;
    class Texture;
    class Camera;
    class RenderTexture;
    class FrameBuffer;
    class DeviceResource;

    enum RenderQueueType
    {
        Opacity = 0,
        Transparent
    };

    class Graphic
    {
    public:
        
        enum class API
        {
            None,
            OpenGL,
        };
        
        Graphic(API api);
        ~Graphic() = default;
        
        void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height);
        void SetClearColor(const glm::vec4& color);
        void Clear(uint8_t flag);
        
        void SetRenderTexture(const std::shared_ptr<RenderTexture>& renderTexture, bool binding = true);
        
        void DrawMesh(Mesh* mesh, Material* material, Camera* camera, uint8_t submesh, uint8_t passID, const glm::mat4& modelMatrix);
        
        void Finish();
        
        void Begin();
        void End();
    
    private:
        
        struct RenderCommand
        {
            Mesh* Mesh_;
            Material* Material_;
            Camera* Camera_;
            uint8_t SubMesh;
            uint8_t Pass;
            glm::mat4 ModelMatrix;
            float SortKey;
        };
        
        std::shared_ptr<VertexArray> getVertexArray(Mesh* mesh, uint8_t submesh);
        std::shared_ptr<Program> getProgram(Material* material, uint32_t passID);
        std::shared_ptr<DeviceTexture> getDeviceTextureBy(Texture* tex);
        
        void drawCommand(const RenderCommand& cmd);
        
    private:
        
        std::unique_ptr<RenderDevice> m_renderDevice;
        
        std::map<uint8_t, std::vector<RenderCommand>> m_renderQueue;
        
        std::unordered_map<std::size_t, std::shared_ptr<VertexBuffer>>  m_vboList;
        std::unordered_map<std::size_t, std::shared_ptr<VertexArray>>   m_vaoList;
        std::unordered_map<std::size_t, std::shared_ptr<Program>>       m_programList;
        std::unordered_map<std::size_t, std::shared_ptr<DeviceTexture>> m_deviceTextureList;
        std::unordered_map<std::size_t, std::shared_ptr<FrameBuffer>>   m_frameBufferList;
    };

}

#endif /* Graphic_hpp */
