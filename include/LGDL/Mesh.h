#pragma once

#include <vector>

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
}
