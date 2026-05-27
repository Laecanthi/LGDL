#pragma once

#include <vector>

#include <LGDL/Mesh.h>
#include <LGDL/Types.h>
#include <LGDL/Camera.h>

namespace LGDL
{
    class Renderer
    {
    public:

        // CORE

        void Initialize();
        void AttributeSetup(const Batch& batch);
        void BeginFrame(const Camera& cam);
        void Flush();
        void DrawBatch(const Batch& batch);

        // DRAW

        void DrawRect(const Transform& transform, const Color& color);
        void DrawTriangle(const Transform& transform, const Color& color);
        void DrawLine(const Vec2& start, const Vec2& end, const float& thickness, const Color& color);
        void DrawVector(const Vec2& position, const Vec2& vector, const float& thickness, const Color& color);

    private:

        GLuint shaderProgram;
        GLuint instanceVBO;

        // batches

        Batch rectBatch;
        Batch triBatch;
    };
}