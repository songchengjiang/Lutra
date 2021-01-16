//
//  TextureBuilder.hpp
//  Editor
//
//  Created by JasonCheng on 2021/1/13.
//

#ifndef TextureBuilder_hpp
#define TextureBuilder_hpp
#include <Lutra.h>

namespace LutraEditor {

    class TextureBuilder
    {
    public:
        
        static std::shared_ptr<Lutra::Texture> Build(const std::string& path);
    };

    class RenderTextureBuilder
    {
    public:
        
        static std::shared_ptr<Lutra::Texture> Build(uint32_t width, uint32_t height);
    };

}

#endif /* TextureBuilder_hpp */
