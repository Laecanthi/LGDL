#include <LGDL/Primitive.h>

namespace LGDL
{
    std::vector<Vertex> CreateRectangleMesh()
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
    }
}

