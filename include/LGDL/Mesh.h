#pragma once

#include <vector>
#include <functional>

#include <LGDL/Types.h>
#include <LGDL/Graphics.h>

namespace LGDL
{
    Mesh UploadMesh(const VertexMesh& vertexMesh);
    
    void ApplyTransform(VertexMesh& mesh, const Transform& transform);

    void ApplyFragData(VertexMesh& mesh, const Color& color, const std::vector<Vec2>& uv = {});

    std::vector<Vec2> CalculateUVs(const VertexMesh& mesh);

    VertexMesh CalculateAndApplyUVs (VertexMesh& mesh);

    void SetupPrimitive(VertexMesh& mesh, const Transform& transform, const Color& color, bool calculateUVs = false);

    VertexMesh JoinMesh(const VertexMesh& target, const VertexMesh& source);

    void AppendMesh(VertexMesh& target, const VertexMesh& source);

    
    std::vector<Vec2> CreateVerticesFromEquation(
        std::function<float(float)> func,
        float min, float max,
        int minSamples, float fidelity, int maxDepth);

    void Subdivide(
        const std::function<float(float)>& func,
        float x1, float y1,
        float x2, float y2,
        float fidelity,
        int depth,
        int maxDepth,
        std::vector<Vec2>& out
    );
}
