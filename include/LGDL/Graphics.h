#pragma once

#include <vector>

#include <LGDL/Types.h>

namespace LGDL
{
    struct Vertex
    {
        // vertex shader info
        Vec2 pos;

        // frag shader info
        Color col;
        Vec2 uv;

        Vertex(
            const Vec2& p,
            const Color& c = {0,0,0,1},
            const Vec2& u = {0,0}
        )
            : pos(p), col(c), uv(u)
        {}

        void SetFragData(const Color& c, const Vec2& v)
        {
            col = c;
            uv = v;
        }
    };

    struct Mesh
    {
        GLuint VAO;
        GLuint VBO;

        int vertexCount;
    };

    struct VertexMesh
    {
        std::vector<Vertex> vertices;
    };

    struct InstanceData
    {
        Transform transform;
        Color color;
    };

    struct InstanceBatch
    {
        Mesh mesh;
        std::vector<InstanceData> instances;
    };

    struct GeometryBatch
    {
        std::vector<Vertex> vertices;

        GLuint VAO;
        GLuint VBO;
    };

    struct DrawCommand
    {
        VertexMesh mesh;
        int layer;
    };
}