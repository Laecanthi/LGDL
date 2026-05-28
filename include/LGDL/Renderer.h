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
        void InstancedAttributeSetup(const InstanceBatch& batch);
        void AttributeSetup(const GeometryBatch& batch);
        void BeginFrame(const Camera& cam);
        void Flush();
        void DrawInstanceBatch(const InstanceBatch& batch);
        void DrawGeometryBatch(const GeometryBatch& batch);

        // DRAW

        void DrawMesh(const VertexMesh& mesh);
        void PushTriangle(const Vec2& v1, const Vec2& v2, const Vec2& v3);

        void DrawRect(const Transform& transform, const Color& color);
        void DrawTriangle(const Transform& transform, const Color& color);
        void DrawCircle(int res, const Transform& transform, const Color& color);
        
        void DrawLine(const Vec2& start, const Vec2& end, const float& thickness, const Color& color);
        void DrawVector(const Vec2& position, const Vec2& vector, const float& thickness, const Color& color);

    private:

        GLuint shaderProgram;
        GLuint instanceVBO;

        // batches

        InstanceBatch rectBatch;
        InstanceBatch triBatch;

        GeometryBatch geometryBatch;
    };
}