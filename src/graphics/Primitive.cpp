#include <LGDL/Primitive.h>

namespace LGDL
{
    /*std::vector<Vertex> CreateRectangleMesh()
    {
        std::vector<Vertex> v;

        // triangle 1
        v.push_back({-0.5f, -0.5f});
        v.push_back({ 0.5f, -0.5f});
        v.push_back({ 0.5f,  0.5f});

        // triangle 2
        v.push_back({-0.5f, -0.5f});
        v.push_back({ 0.5f,  0.5f});
        v.push_back({-0.5f,  0.5f});

        return v;
    }

    std::vector<Vertex> CreateTriangleMesh()
    {
        std::vector<Vertex> v;

        v.push_back({0, 0}); // tip of triangle
        v.push_back({-1, 0.5});
        v.push_back({-1, -0.5});

        return v;
    }*/

    VertexMesh PrimitiveRect()
    {
        return JoinMesh(
                CreateTriangle({-0.5f, -0.5f}, {0.5f, -0.5f}, {0.5f, 0.5f}),
                CreateTriangle({-0.5f, -0.5f},{0.5f, 0.5f}, {-0.5f, 0.5f})
            );
    }

    VertexMesh PrimitiveTriangle()
    {
        return CreateTriangle({0,0}, {-1, 0.5}, {-1, -0.5});
    }


    VertexMesh CreateTriangle(const Vec2& v1, const Vec2& v2, const Vec2& v3)
    {
        VertexMesh mesh;

        mesh.vertices.push_back(Vertex(v1));
        mesh.vertices.push_back(Vertex(v2));
        mesh.vertices.push_back(Vertex(v3));

        return mesh;
    }
}

