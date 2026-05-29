#pragma once

#include <vector>

#include <LGDL/Mesh.h>
#include <LGDL/Graphics.h>

namespace LGDL
{
    VertexMesh PrimitiveRect();

    VertexMesh PrimitiveTriangle();

    VertexMesh PrimitiveArc(int res, float startAngle, float endAngle);

    VertexMesh PrimitiveLineArc(int res, float thickness, float startAngle, float endAngle);

    VertexMesh PrimitiveCircle(int res);

    VertexMesh CreateTriangle(const Vec2& v1, const Vec2& v2, const Vec2& v3);

    VertexMesh CreateRect(const Vec2& v1, const Vec2& v2, const Vec2& v3, const Vec2& v4);
}
