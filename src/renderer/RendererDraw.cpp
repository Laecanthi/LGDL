#include <LGDL/Renderer.h>

#include <iostream>

#include <LGDL/Primitive.h>
#include <LGDL/Shader.h>
#include <LGDL/Math.h>

namespace LGDL
{
    void Renderer::DrawRect(const Transform& transform, const Color& color)
    {
        rectBatch.instances.push_back({transform, color});
    }

    void Renderer::DrawTriangle(const Transform& transform, const Color& color)
    {
        triBatch.instances.push_back({transform, color});
    }

    void Renderer::DrawLine(const Vec2& start, const Vec2& end, const float& thickness, const Color& color)
    {
        Transform transform;

        Vec2 delta = end - start;

        transform.position = start + (delta * 0.5);

        transform.scale.x = VMag(delta);

        transform.scale.y = thickness;

        transform.rotation = VDir(delta);

        rectBatch.instances.push_back({transform, color});
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