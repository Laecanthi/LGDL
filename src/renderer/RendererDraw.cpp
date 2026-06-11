#include <LGDL/Renderer.h>

#include <iostream>

#include <LGDL/Primitive.h>
#include <LGDL/Shader.h>
#include <LGDL/Math.h>
#include <LGDL/Glyphs.h>

namespace LGDL
{
    void Renderer::DrawMesh(const VertexMesh& mesh)
    {
        DrawCommand cmd;

        cmd.mesh = mesh;

        renderTargets[drawState.target]
            .commands
            .push_back(RenderCommand(cmd, drawState.layer));
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

            //std::cout << "New circle res was cached: " << res << "! \n";

            VertexMesh circle = PrimitiveCircle(res);

            circle = CalculateAndApplyUVs(circle); // btw this function returns the mesh in scenarios where there's no original mesh to have modified in the first place

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

    void Renderer::Write(const std::string& text, Vec2 pos, float size, Font& font, const Color& color)
    {
        Vec2 penPosition = pos;

        penPosition += {0, font.lineHeight / font.defaultFontSize * size};
        
        for(char c : text)
        {
            Glyph glyph = font.glyphs[static_cast<int>(c)];

            InstanceData instance;

            instance.transform.position = penPosition;
            instance.transform.position += {
                (glyph.offset.x  / font.defaultFontSize) * size,
                (-glyph.offset.y  / font.defaultFontSize) * size
            };

            instance.transform.scale = {
                (glyph.size.x  / font.defaultFontSize) * size,
                (-glyph.size.y  / font.defaultFontSize) * size
            };

            instance.transform.rotation = 0;

            instance.minUV = glyph.uvMin;
            instance.maxUV = glyph.uvMax;

            instance.color = color;

            renderTargets[drawState.target].instanceBatch.instances.push_back(instance);

            penPosition.x += glyph.advance / font.defaultFontSize * size;

            /*std::cout
                << glyph.offset.x << ", "
                << glyph.offset.y << ", "
                << glyph.size.x << ", "
                << glyph.size.y << "\n";*/
        }
        #if LGDL_GL_DEBUG
        // scan instance data for non-finite values before uploading
        if (!renderTargets[drawState.target].instanceBatch.instances.empty()) {
            const float* fptr = reinterpret_cast<const float*>(renderTargets[drawState.target].instanceBatch.instances.data());
            size_t count = (renderTargets[drawState.target].instanceBatch.instances.size() * sizeof(InstanceData)) / sizeof(float);
            for (size_t i = 0; i < count; ++i) {
                if (!std::isfinite(fptr[i])) {
                    std::cerr << "[GL DEBUG] non-finite in InstanceData at float index " << i << "\n";
                    break;
                }
            }
        }
        #endif
    }

}