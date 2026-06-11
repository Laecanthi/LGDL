#pragma once

#include <vector>

namespace LGDL
{
    using GLuint = unsigned int;
    
    struct Vec2
    {
        float x;
        float y;
    };

    struct Mat3
    {
        float m[3][3];
    };

    struct Color
    {
        float r;
        float g;
        float b;
        float a;
    };

    struct Transform
    {
        Vec2 position;
        Vec2 scale;
        float rotation;
    };
}
