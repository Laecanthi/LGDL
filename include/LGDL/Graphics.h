#pragma once

#include <vector>
#include <unordered_map>
#include <variant>

#include <LGDL/Types.h>
#include <LGDL/Texture.h>

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

        // UVs are between 0 and 1

        Vec2 minUV; // for no change: (0,0)
        Vec2 maxUV; // for no change: (1,1)
    };

    struct InstanceBatch
    {
        Mesh mesh;
        std::vector<InstanceData> instances;
        
        Texture texture;
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
        //int layer;
    };

    struct RenderCommand
    {
        std::variant<InstanceData, DrawCommand> command;
        int layer;

        RenderCommand(const InstanceData& i, int l) : command(i), layer(l) {};
        RenderCommand(const DrawCommand& d, int l) : command(d), layer(l) {};
    };

    struct GeometryCache
    {
        VertexMesh rect;
        VertexMesh triangle;

        std::unordered_map<int, VertexMesh> circles;
    };
}