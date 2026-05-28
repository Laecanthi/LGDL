#pragma once

#include <vector>

#include <LGDL/Mesh.h>
#include <LGDL/Graphics.h>

namespace LGDL
{
    VertexMesh PrimitiveRect();

    VertexMesh PrimitiveTriangle();

    VertexMesh CreateTriangle(const Vec2& v1, const Vec2& v2, const Vec2& v3);
}
