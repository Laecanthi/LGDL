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
}

