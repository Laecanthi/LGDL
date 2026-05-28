#include <LGDL/Renderer.h>

#include <iostream>

#include <LGDL/Primitive.h>
#include <LGDL/Shader.h>
#include <LGDL/Math.h>

namespace LGDL
{
    void Renderer::DrawMesh(const VertexMesh& mesh)
    {
        geometryBatch.vertices.insert(
            geometryBatch.vertices.end(),
            mesh.vertices.begin(),
            mesh.vertices.end()
        );
    }

    void Renderer::PushTriangle(const Vec2& v1, const Vec2& v2, const Vec2& v3)
    {
        geometryBatch.vertices.push_back(Vertex(v1));
        geometryBatch.vertices.push_back(Vertex(v2));
        geometryBatch.vertices.push_back(Vertex(v3));
    }

    void Renderer::DrawRect(const Transform& transform, const Color& color)
    {
        VertexMesh rect = PrimitiveRect();
        
        std::vector<Vec2> uv = CalculateUVs(rect);

        ApplyFragData(rect, color, uv);

        ApplyTransform(rect, transform);

        DrawMesh(rect);
    }

    void Renderer::DrawTriangle(const Transform& transform, const Color& color)
    {
        VertexMesh triangle = PrimitiveTriangle();

        std::vector<Vec2> uv = CalculateUVs(triangle);

        ApplyFragData(triangle, color, uv);

        ApplyTransform(triangle, transform);

        DrawMesh(triangle);
    }


    /*

    void Renderer::DrawRect(const Transform& transform, const Color& color)
    {
        rectBatch.instances.push_back({transform, color});
    }

    void Renderer::DrawTriangle(const Transform& transform, const Color& color)
    {
        triBatch.instances.push_back({transform, color});
    }

    */

    void Renderer::DrawLine(const Vec2& start, const Vec2& end, const float& thickness, const Color& color)
    {
        Transform transform;

        Vec2 delta = end - start;

        transform.position = start + (delta * 0.5);

        transform.scale.x = VMag(delta);

        transform.scale.y = thickness;

        transform.rotation = VDir(delta);

        DrawRect(transform, color);
    }

    void Renderer::DrawVector(const Vec2& position, const Vec2& vector, const float& thickness, const Color& color)
    {
        Vec2 end = position + vector;

        //std::cout << end.x << "," << end.y << "\n";

        float mag = VMag(vector);
        
        // ratio of line to head
        float ratio = 0.92f;
        // head size
        float head = 3.5f;

        Vec2 lineStop = (position * (1 - ratio)) + (end * ratio);

        //std::cout << lineStop.x << "," << lineStop.y << "\n";

        DrawLine(position, lineStop, thickness, color);

        Transform transform;

        transform.position = end;
        transform.scale = {mag * (1 - ratio), head * thickness};
        transform.rotation = VDir(vector);

        DrawTriangle(transform, color);
    }
}