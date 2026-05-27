#pragma once

#include <LGDL/Types.h>

namespace LGDL
{
    struct Camera
    {
        Vec2 position;
        float zoom;

        float aspectRatio;
    };
}