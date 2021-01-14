//
//  TextureLoader.hpp
//  Editor
//
//  Created by JasonCheng on 2021/1/9.
//

#ifndef TextureLoader_hpp
#define TextureLoader_hpp
#include <stb_image.h>
#include <glad/glad.h>
#include <unordered_map>
#include <string>

namespace LutraEditor {

    enum class IconType
    {
        Folder = 0,
        
        Texture,
        Material,
        Mesh,
        
        Camera,
        SceneObject,
        Tag,
        Square
    };

    class IconManager
    {
    public:
        IconManager() = default;
        ~IconManager() = default;
        
        static IconManager& Instance();
        
        void LoadTextue(IconType type, const std::string& path);
        GLuint GetTexture(IconType type) { return m_textures[type]; }
        
    private:
        
        std::unordered_map<IconType, GLuint> m_textures;
    };

}

#endif /* TextureLoader_hpp */
