#pragma once

#include <vector>

#include <LGDL/Types.h>

namespace LGDL
{
    struct Mesh
    {
        GLuint VAO;
        GLuint VBO;

        int vertexCount;
    };

    struct InstanceData
    {
        Transform transform;
        Color color;
    };

    struct Batch
    {
        Mesh mesh;
        std::vector<InstanceData> instances;
    };
}