#include <LGDL/Mesh.h>

#include <glad/glad.h>

#include <LGDL/Math.h>

namespace LGDL
{
    Mesh UploadMesh(const VertexMesh& vertexMesh)
    {
        // create mesh

        std::vector<Vertex> vertices = vertexMesh.vertices;

        std::vector<Vec2> UVs = CalculateUVs(vertexMesh);

        for(int i = 0; i < vertices.size(); i++)
        {
            vertices[i].uv = UVs[i];
        }

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

        // UVs

        glVertexAttribPointer(
            2, 2, GL_FLOAT, GL_FALSE,
            sizeof(Vertex),
            (void*)offsetof(Vertex, uv)
        );
        glEnableVertexAttribArray(2);

        // set vertex count
        mesh.vertexCount = (int)vertices.size();

        return mesh;
    }

    void ApplyTransform(VertexMesh& mesh, const Transform& transform)
    {
        bool rotate = std::abs(transform.rotation) > 0.0001f; // does not perform rotation calculations if no rotation

        float s = 0;
        float c = 1;


        if(rotate)
        {
            s = sin(transform.rotation);
            c = cos(transform.rotation);
        }
            

        for(int i = 0; i < mesh.vertices.size(); i++)
        {
            Vertex& v = mesh.vertices[i];

            v.pos = v.pos * transform.scale;

            if(rotate)
            {
                v.pos =
                {
                    v.pos.x * c - v.pos.y * s,
                    v.pos.x * s + v.pos.y * c
                };
            }

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

    VertexMesh CalculateAndApplyUVs (const VertexMesh& m)
    {
        std::vector<Vec2> uv = CalculateUVs(m);

        VertexMesh mesh = m;

        ApplyFragData(mesh, {0,0,0,1}, uv);

        return mesh;

        // for the sake of applying UVs, the default color is black with an alpha of 1
    }

    void SetupPrimitive(VertexMesh& mesh, const Transform& transform, const Color& color, bool calculateUVs)
    {
        if(calculateUVs)
        {
            std::vector<Vec2> uv = CalculateUVs(mesh);

            ApplyFragData(mesh, color, uv);
        }else{
            ApplyFragData(mesh, color);
        }

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




    std::vector<Vec2> CreateVerticesFromEquation(
        std::function<float(float)> func,
        float min, float max,
        int minSamples, float fidelity, int maxDepth)
    {
        std::vector<Vec2> points;

        float scale = max - min;
        float step = scale / minSamples;

        points.push_back({min, func(min)});

        for(int i = 0; i < minSamples; i++)
        {
            float x1 = min + (i * step);
            float x2 = min + ((i+1) * step);
            Subdivide(func, x1, func(x1), x2, func(x2), fidelity, 0, maxDepth, points);
        }   
        
        return points;
    }

    void Subdivide(
        const std::function<float(float)>& func,
        float x1, float y1,
        float x2, float y2,
        float fidelity,
        int depth,
        int maxDepth,
        std::vector<Vec2>& out
    )
    {
        float xm = (x1 + x2) * 0.5f;
        float ym = func(xm);

        float yLinear = (y1 + y2) * 0.5f;

        float error = fabs(ym - yLinear);

        if (error > fidelity && depth < maxDepth)
        {
            Subdivide(func, x1, y1, xm, ym, fidelity, depth + 1, maxDepth, out);
            Subdivide(func, xm, ym, x2, y2, fidelity, depth + 1, maxDepth, out);
            return;
        }

        out.push_back({x2, y2});
    }
}