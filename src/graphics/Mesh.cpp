#include <LGDL/Mesh.h>

#include <glad/glad.h>

#include <LGDL/Math.h>

namespace LGDL
{
    Mesh UploadMesh(const std::vector<Vertex>& vertices)
    {
        // create mesh

        Mesh mesh;

        glGenVertexArrays(1, &mesh.VAO);
        glGenBuffers(1, &mesh.VBO);

        glBindVertexArray(mesh.VAO);
        glBindBuffer(GL_ARRAY_BUFFER, mesh.VBO);

        glBufferData(
            GL_ARRAY_BUFFER,
            vertices.size() * sizeof(Vertex),
            vertices.data(),
            GL_STATIC_DRAW
        );

        // position
        glVertexAttribPointer(
            0, 2, GL_FLOAT, GL_FALSE,
            sizeof(Vertex),
            (void*)offsetof(Vertex, pos)
        );
        glEnableVertexAttribArray(0);

        // set vertex count
        mesh.vertexCount = (int)vertices.size();

        return mesh;
    }

    void ApplyTransform(VertexMesh& mesh, const Transform& transform)
    {
        float s = sin(transform.rotation);
        float c = cos(transform.rotation);

        for(int i = 0; i < mesh.vertices.size(); i++)
        {
            Vertex& v = mesh.vertices[i];

            v.pos = v.pos * transform.scale;

            v.pos = {v.pos.x * c - v.pos.y * s, v.pos.x * s + v.pos.y * c};

            v.pos += transform.position;
        }
    }

    void ApplyFragData(VertexMesh& mesh, const Color& color, const std::vector<Vec2>& uv)
    {
        for(int i = 0; i < mesh.vertices.size(); i++)
        {
            Vertex& v = mesh.vertices[i];
            if(uv.empty())
            {
                v.SetFragData(color, {0,0});
            }else{
                v.SetFragData(color, uv[i]);
            }
        }
    }

    std::vector<Vec2> CalculateUVs(const VertexMesh& mesh)
    {
        std::vector<Vec2> UVs;
        Vec2 mi = mesh.vertices[0].pos;
        Vec2 ma = mesh.vertices[0].pos;

        // get bounds of vertices

        for(int i = 0; i < mesh.vertices.size(); i++)
        {
            Vertex v = mesh.vertices[i];
            
            mi.x = min(mi.x, v.pos.x);
            mi.y = min(mi.y, v.pos.y);

            ma.x = max(ma.x, v.pos.x);
            ma.y = max(ma.y, v.pos.y);
        }

        Vec2 size = ma - mi;

        for(int i = 0; i < mesh.vertices.size(); i++)
        {
            Vertex v = mesh.vertices[i];
            Vec2 uv;

            uv.x = (v.pos.x - mi.x) / size.x;
            uv.y = (v.pos.y - mi.y) / size.y;

            UVs.push_back(uv);
        }

        return UVs;
    }

    void SetupPrimitive(VertexMesh& mesh, const Transform& transform, const Color& color)
    {
        std::vector<Vec2> uv = CalculateUVs(mesh);

        ApplyFragData(mesh, color, uv);

        ApplyTransform(mesh, transform);

        // a helper function that turns a primitive into a mesh
    }

    VertexMesh JoinMesh(const VertexMesh& target, const VertexMesh& source)
    {
        VertexMesh output = target;
        output.vertices.insert(
            output.vertices.end(),
            source.vertices.begin(),
            source.vertices.end()
        );
        return output;
    }

    void AppendMesh(VertexMesh& target, const VertexMesh& source)
    {
        target.vertices.insert(
            target.vertices.end(),
            source.vertices.begin(),
            source.vertices.end()
        );
    }
}