#include <LGDL/Primitive.h>

#include <LGDL/Math.h>

namespace LGDL
{

    VertexMesh PrimitiveRect()
    {
        return CreateRect({-0.5, 0.5}, {0.5, 0.5}, {-0.5, -0.5}, {0.5, -0.5});
    }

    VertexMesh PrimitiveTriangle()
    {
        return CreateTriangle({0,0}, {-1, 0.5}, {-1, -0.5});
    }

    VertexMesh PrimitiveArc(int res, float startAngle, float endAngle)
    {
        VertexMesh arc;

        arc.vertices.reserve(res * 3);


        float angle = (endAngle - startAngle) / res;

        for(int i = 0; i < res; i++)
        {
            AppendMesh(
                arc,
                CreateTriangle({0,0}, {cos(angle * i + startAngle), sin(angle * i + startAngle)}, {cos(angle * (i+1) + startAngle), sin(angle * (i+1) + startAngle)})
            );
        }

        return arc;
    }

    VertexMesh PrimitiveLineArc(int res, float thickness, float startAngle, float endAngle)
    {
        VertexMesh arc;

        arc.vertices.reserve(res * 3);


        float angle = (endAngle - startAngle) / res;

        for(int i = 0; i < res; i++)
        {
            Vec2 thisPoint = {cos(angle * i + startAngle), sin(angle * i + startAngle)};
            Vec2 nextPoint = {cos(angle * (i+1) + startAngle), sin(angle * (i+1) + startAngle)};

            AppendMesh(
                arc,
                CreateRect(thisPoint, thisPoint * (1 - thickness), nextPoint, nextPoint * (1 - thickness))
            );
        }

        return arc;
    }

    VertexMesh PrimitiveCircle(int res)
    {
        VertexMesh circle;

    circle.vertices.reserve(res * 3);

        float angle = (2 * PI) / res;

        for(int i = 0; i < res; i++)
        {
            AppendMesh(
                circle,
                CreateTriangle({0,0}, {cos(angle * i), sin(angle * i)}, {cos(angle * (i+1)), sin(angle * (i+1))})
            );
        }

        return circle;
    }


    VertexMesh CreateTriangle(const Vec2& v1, const Vec2& v2, const Vec2& v3)
    {
        VertexMesh mesh;

        mesh.vertices.push_back(Vertex(v1));
        mesh.vertices.push_back(Vertex(v2));
        mesh.vertices.push_back(Vertex(v3));

        return mesh;
    }

    VertexMesh CreateRect(const Vec2& v1, const Vec2& v2, const Vec2& v3, const Vec2& v4)
    {
        VertexMesh mesh;

        mesh.vertices.push_back(Vertex(v1));
        mesh.vertices.push_back(Vertex(v2));
        mesh.vertices.push_back(Vertex(v3));

        mesh.vertices.push_back(Vertex(v2));
        mesh.vertices.push_back(Vertex(v3));
        mesh.vertices.push_back(Vertex(v4));

        return mesh;
    }
}

