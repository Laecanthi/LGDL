#pragma once

#include <string>

namespace LGDL
{
    struct Texture
    {
        unsigned int ID;

        int width;
        int height;

        int channels;
    };

    Texture LoadTexture(const std::string& path);
}