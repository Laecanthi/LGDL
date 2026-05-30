#include <LGDL/Renderer.h>

#include <iostream>

#include <LGDL/Primitive.h>
#include <LGDL/Shader.h>
#include <LGDL/Math.h>

namespace LGDL
{
    void Renderer::DrawMesh(const VertexMesh& mesh)
    {
        DrawCommand cmd;

        cmd.mesh = mesh;
        cmd.layer = drawState.layer;

        renderTargets[drawState.target]
            .commands
            .push_back(cmd);
    }

    /*void Renderer::PushTriangle(const Vec2& v1, const Vec2& v2, const Vec2& v3) // be aware that this does NOT give any information to the frag shader
    {
        geometryBatch.vertices.push_back(Vertex(v1));
        geometryBatch.vertices.push_back(Vertex(v2));
        geometryBatch.vertices.push_back(Vertex(v3));
    }*/

    void Renderer::DrawRect(const Transform& transform, const Color& color)
    {
        VertexMesh rect = geometryCache.rect;
        
        SetupPrimitive(rect, transform, color);

        DrawMesh(rect);
    }

    void Renderer::DrawRect(const Vec2& pos, const Vec2& size, const Color& color) // draws rect from bottom left corner rather than center
    {
        VertexMesh rect = geometryCache.rect;

        Transform transform;

        transform.scale = size;

        Vec2 end = pos + size;

        transform.position = (pos + end) * 0.5;

        transform.rotation = 0;

        SetupPrimitive(rect, transform, color);

        DrawMesh(rect);
    }

    void Renderer::DrawTriangle(const Transform& transform, const Color& color)
    {
        VertexMesh triangle = geometryCache.triangle;

        /*std::vector<Vec2> uv = CalculateUVs(triangle);

        ApplyFragData(triangle, color, uv);

        ApplyTransform(triangle, transform);*/

        SetupPrimitive(triangle, transform, color);

        DrawMesh(triangle);
    }

    void Renderer::DrawCircle(int res, const Transform& transform, const Color& color)
    {
        auto it = geometryCache.circles.find(res);

        if(it != geometryCache.circles.end())
        {
            VertexMesh circle = it->second;

            SetupPrimitive(circle, transform, color);

            DrawMesh(circle);
        }else{

            std::cout << "New circle res was cached: " << res << "! \n";

            VertexMesh circle = PrimitiveCircle(res);

            CalculateAndApplyUVs(circle); // btw this function returns the mesh in scenarios where there's no original mesh to have modified in the first place

            geometryCache.circles[res] = circle; 

            SetupPrimitive(circle, transform, color, false); // for context the default value for CalculateUVs is false

            DrawMesh(circle);
        }
    }

    void Renderer::DrawArc(int res, float startAngle, float endAngle, const Transform& transform, const Color& color)
    {
        VertexMesh arc = PrimitiveArc(res, startAngle, endAngle);

        SetupPrimitive(arc, transform, color, true);

        DrawMesh(arc);
    }

    void Renderer::DrawLineArc(int res, float thickness, float startAngle, float endAngle, const Transform& transform, const Color& color)
    {
        VertexMesh arc = PrimitiveLineArc(res, thickness, startAngle, endAngle);

        SetupPrimitive(arc, transform, color, true);

        DrawMesh(arc);
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

    void Renderer::DrawVectorRef(const Vec2& position, const Vec2& vector, const float& ref, const float& thickness, const Color& color)
    {
        // ratio of vector to reference
        float ratio = 0.4f;

        float size = VMag(vector) * ratio;

        size = min(max(size, 0.1), VMag(vector));

        Transform transform;
        transform.position = position;
        transform.scale = {size, size};
        transform.rotation = 0;

        DrawLineArc(50, thickness / size, ref, VDir(vector), transform, color);

        float referenceSize = size / 5;

        Vec2 reference = Polar(ref, size) + position;

        DrawLine(reference - Polar(ref, referenceSize + thickness), reference + Polar(ref, referenceSize), thickness, color);
    }

    void Renderer::DrawRefVector(const Vec2& position, const Vec2& vector, const float& ref, const float& thickness, const Color& color)
    {
        DrawVectorRef(position, vector, ref, thickness, color);

        DrawVector(position, vector, thickness, color);
    }

    void Renderer::Write()
    {
        InstanceData d;
        d.transform = {
            {0,0},
            {1,1},
            0
        };
        d.color = {1,1,1,1};

        textBatch.instances.push_back(d);
    }

}