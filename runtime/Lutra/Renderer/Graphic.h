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
#include "RenderDevice.h"

namespace Lutra {

    struct Mesh;
    class Material;
    class Texture;
    class Camera;
    class RenderTarget;
    class FrameBuffer;

    struct HashFunc
    {
        std::size_t operator()(const std::pair<Mesh*, uint8_t> &key) const
        {
            using std::size_t;
            using std::hash;
     
            return hash<Mesh*>()(key.first) + hash<uint8_t>()(key.second);
        }
        
        std::size_t operator()(const std::pair<Material*, uint8_t> &key) const
        {
            using std::size_t;
            using std::hash;
     
            return hash<Material*>()(key.first) + hash<uint8_t>()(key.second);
        }
    };

    struct EqualKey
    {
        bool operator () (const std::pair<Mesh*, uint8_t> &lhs, const std::pair<Mesh*, uint8_t> &rhs) const
        {
            return lhs.first == rhs.first && lhs.second == rhs.second;
        }
        
        bool operator () (const std::pair<Material*, uint8_t> &lhs, const std::pair<Material*, uint8_t> &rhs) const
        {
            return lhs.first == rhs.first && lhs.second == rhs.second;
        }
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
        
        void SetRenderTarget(std::shared_ptr<RenderTarget>& renderTarget);
        
        void DrawMesh(Mesh* mesh, Material* material, Camera* camera, const glm::mat4& modelMatrix, uint8_t submesh);
        
        void Begin();
        void End();
    
    private:
        
        std::shared_ptr<VertexArray> updateVertexArray(Mesh* mesh, uint8_t submesh);
        std::shared_ptr<Program> updateProgram(Material* material, uint32_t passID);
        std::shared_ptr<DeviceTexture> getDeviceTextureBy(Texture* tex);
        
    private:
        
        std::unique_ptr<RenderDevice> m_renderDevice;
        
        std::unordered_map<std::pair<Mesh*, uint8_t>, std::shared_ptr<VertexArray>, HashFunc, EqualKey> m_meshList;
        std::unordered_map<std::pair<Material*, uint8_t>, std::shared_ptr<Program>, HashFunc, EqualKey> m_materialList;
        std::unordered_map<Texture*, std::shared_ptr<DeviceTexture>>                                    m_textureList;
        std::unordered_map<RenderTarget*, std::shared_ptr<FrameBuffer>>                                 m_renderTargetList;
    };

}

#endif /* Graphic_hpp */
