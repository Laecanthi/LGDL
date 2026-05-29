#pragma once

#include <vector>
#include <unordered_map>

#include <LGDL/Mesh.h>
#include <LGDL/Types.h>
#include <LGDL/Graphics.h>
#include <LGDL/Camera.h>

namespace LGDL
{
    struct DrawState
    {
        int target;
        int layer;
    };

    struct RenderTarget
    {
        std::vector<DrawCommand> commands;

        GeometryBatch geometryBatch;

        GLuint shaderProgram;
    };

    class Renderer
    {
    public:

        // CORE

        void Initialize();
        void InstancedAttributeSetup(const InstanceBatch& batch);
        void AttributeSetup(GeometryBatch& batch);
        void BeginFrame(const Camera& cam, const Screen& screen);
        void Flush();
        void DrawInstanceBatch(const InstanceBatch& batch);
        void DrawGeometryBatch(const GeometryBatch& batch);
        void SetState(int target, int layer);
        void SortCommands();

        // DRAW

        void DrawMesh(const VertexMesh& mesh);
        void PushTriangle(const Vec2& v1, const Vec2& v2, const Vec2& v3);

        void DrawRect(const Transform& transform, const Color& color);
        void DrawRect(const Vec2& pos, const Vec2& size, const Color& color);

        void DrawTriangle(const Transform& transform, const Color& color);

        void DrawCircle(int res, const Transform& transform, const Color& color);

        void DrawArc(int res, float startAngle, float endAngle, const Transform& transform, const Color& color);
        void DrawLineArc(int res, float thickness, float startAngle, float endAngle, const Transform& transform, const Color& color);

        void DrawLine(const Vec2& start, const Vec2& end, const float& thickness, const Color& color);

        void DrawVector(const Vec2& position, const Vec2& vector, const float& thickness, const Color& color);
        void DrawVectorRef(const Vec2& position, const Vec2& vector, const float& ref, const float& thickness, const Color& color);
        void DrawRefVector(const Vec2& position, const Vec2& vector, const float& ref, const float& thickness, const Color& color);


        //void DrawVector(const Vec2& position, const float angle, const float mag, const float& thickness, const Color& color);

    private:

        DrawState drawState;

        GLuint SPprimitiveWorld;
        GLuint SPprimitiveUI;
        GLuint instanceVBO;

        std::vector<int> renderOrder;
        std::unordered_map<int, RenderTarget> renderTargets;

        // batches

        //InstanceBatch rectBatch;
        //InstanceBatch triBatch;

        //std::unordered_map<int, std::vector<DrawCommand>> commands;

        //GeometryBatch worldBatch; // target 0
        //GeometryBatch uiBatch; // target 1
    };
}