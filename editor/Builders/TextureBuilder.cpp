//
//  TextureBuilder.cpp
//  Editor
//
//  Created by JasonCheng on 2021/1/13.
//

#include "TextureBuilder.h"
#include <stb_image.h>

namespace LutraEditor {

    std::shared_ptr<Lutra::Texture> TextureBuilder::Build(const std::string& path)
    {
        int width, height, channels;
        stbi_set_flip_vertically_on_load(1);
        stbi_uc* data = nullptr;
        {
            data = stbi_load(path.c_str(), &width, &height, &channels, 0);
        }
        
        auto texture = Lutra::ResourceManager::Instance().CreateResource<Lutra::Texture2D>(width, height, channels == 3? Lutra::TextureFormat::RGB8: Lutra::TextureFormat::RGBA8);
        texture->SetData(data, width * height * channels);
        stbi_image_free(data);
        return texture;
    }

}
