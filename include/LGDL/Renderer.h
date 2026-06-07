#pragma once

#include <vector>
#include <unordered_map>
#include <variant>
//#include <deque>

#include <LGDL/Mesh.h>
#include <LGDL/Types.h>
#include <LGDL/Graphics.h>
#include <LGDL/Camera.h>
#include <LGDL/Glyphs.h>

namespace LGDL
{

    // STRUCTS: The structs that go here in Renderer.h either are GLAD specific or Renderer specific
        // Anything that may be needed to be used for non renderer purposes (say, in Mesh.h) do not belong here

    struct DrawState
    {
        int target;
        int layer;
    };

    enum class Uniform
    {
        View,
        Screen,
        Texture0
    };

    inline const char* UniformName(Uniform u) // THIS is the list of uniform enum -> names
    {
        //apparently the function wants a GLchar but that's undefined so uh I'll just use char and pray there's no errors

        switch(u)
        {
            case Uniform::View:
                return "uView";

            case Uniform::Screen:
                return "uScreen";

            case Uniform::Texture0:
                return "uTexture0";
        }

        return "";
    }


    struct ShaderProgram
    {
        GLuint ID;

        std::unordered_map<Uniform, GLuint> uniformLocations;

        std::vector<Uniform> uniforms; // this can just be a list, order doesn't actually matter since this is just for initialization

        GLuint GetUniformLocation(Uniform u) const
        {
            return uniformLocations.at(u);
        }

        bool HasUniform(Uniform u) const
        {
            return uniformLocations.contains(u);
        }
    };

    struct RenderTarget
    {
        std::vector<RenderCommand> commands; // RenderCommand is an std::varient
            // can be either a DrawCommand (goes into geometryBatch)
                // draw commands must be flattened before going into the batch
            // or InstanceData (goes into instanceBatch)
                // instance data can go directly into the batch

        // note: I could probably make the batches also a varient, because of how shaders get assigned there can't be both anyways
            // for now, they will stay seperate. However; things will break if attempting to use both batch types in the same target
            // because of that, it probably wouldn't be a bad idea to clarify what type of batch each RenderTarget uses

        GeometryBatch geometryBatch;
            // requires a VBO, VAO, and a list of Vertex (vertices)

        InstanceBatch instanceBatch;
            // requires a Mesh (mesh), Texture (texture), and list of InstanceData (instances)

        ShaderProgram* program = nullptr; // points to the address of the shader program, does not own program
            // render target should never edit program, only read from it
    };


    class Renderer /******************************* RENDERER CLASS *******************************************/
    {
    public:

        // CORE (RendererCore.cpp)

        void Initialize();
        void InstancedAttributeSetup(const InstanceBatch& batch);
        void AttributeSetup(GeometryBatch& batch);
        void UploadUniforms(ShaderProgram& shader, const Camera& cam, const Screen& screen);
        void BeginFrame(const Camera& cam, const Screen& screen);
        void Flush();
        void DrawInstanceBatch(const InstanceBatch& batch, const ShaderProgram& shader);
        void DrawGeometryBatch(const GeometryBatch& batch);
        void SetState(int target, int layer);
        void SortCommands();

        // DRAW (RendererDraw.cpp)

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

            // (text)

        void Write(const std::string& text, Vec2 pos, float size, Font& font, const Color& color);


        //void DrawVector(const Vec2& position, const float angle, const float mag, const float& thickness, const Color& color);

        std::unordered_map<int, RenderTarget> renderTargets;
        std::unordered_map<int, ShaderProgram> shaderPrograms;

    private:

        DrawState drawState;

        GeometryCache geometryCache;

        

        //GLuint SPprimitiveWorld;
        //GLuint SPprimitiveUI;
        //GLuint SPprimitiveSprite;
        
        GLuint instanceVBO;

        std::vector<int> renderOrder;
        

        // batches

        //InstanceBatch textBatch;

        //InstanceBatch rectBatch;
        //InstanceBatch triBatch;

        //std::unordered_map<int, std::vector<DrawCommand>> commands;

        //GeometryBatch worldBatch; // target 0
        //GeometryBatch uiBatch; // target 1
    };
}