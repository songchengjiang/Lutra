//
//  TextureManager.hpp
//  Editor
//
//  Created by JasonCheng on 2021/1/12.
//

#ifndef TextureManager_hpp
#define TextureManager_hpp
#include "Lutra.h"
#include <glad/glad.h>
#include <vector>

namespace LutraEditor {

    class TextureManager
    {
    public:
        TextureManager() = default;
        ~TextureManager() = default;
        
        static TextureManager& Instance();
        
        uint32_t CreateTextue(uint32_t width, uint32_t height, Lutra::TextureFormat format, const uint8_t* data);
        
    private:
        
        std::vector<uint32_t> m_textures;
    };

}

#endif /* TextureManager_hpp */
