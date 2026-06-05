#include <LGDL/Renderer.h>

#include <iostream>
#include <glad/glad.h>
#include <algorithm>
#include <variant>

#include <LGDL/Primitive.h>
#include <LGDL/Shader.h>
#include <LGDL/Math.h>
#include <LGDL/Texture.h>

namespace LGDL
{
    void Renderer::Initialize()
    {
        //rectBatch.mesh = UploadMesh(CreateRectangleMesh()); // creates the rectangle mesh
        //triBatch.mesh = UploadMesh(CreateTriangleMesh()); // creates the triangle mesh

        SetState(0,0);

        renderOrder.push_back(0); // world
        renderOrder.push_back(5); // world text
        renderOrder.push_back(10); // ui
        renderOrder.push_back(15); // ui text

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glEnable(GL_MULTISAMPLE);

        int MAX_INSTANCES = 5000; // arbitrary max instances being 5k.
        int MAX_VERTICES = 50000; // arbitraryt max vertices being 50k.

        // create VBO
        glGenBuffers(1, &instanceVBO);
        glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(InstanceData) * MAX_INSTANCES, nullptr, GL_DYNAMIC_DRAW);

        // cache primitives

        geometryCache.rect = CalculateAndApplyUVs(PrimitiveRect());
        geometryCache.triangle = CalculateAndApplyUVs(PrimitiveTriangle());
        geometryCache.circles[3] =  CalculateAndApplyUVs(PrimitiveCircle(3));
        geometryCache.circles[4] =  CalculateAndApplyUVs(PrimitiveCircle(4));
        geometryCache.circles[6] =  CalculateAndApplyUVs(PrimitiveCircle(6));
        geometryCache.circles[12] =  CalculateAndApplyUVs(PrimitiveCircle(12));
        geometryCache.circles[24] =  CalculateAndApplyUVs(PrimitiveCircle(24));
        geometryCache.circles[50] =  CalculateAndApplyUVs(PrimitiveCircle(50));
        geometryCache.circles[100] =  CalculateAndApplyUVs(PrimitiveCircle(100));


        // setup attributes
        for(int target : renderOrder)
        {
           AttributeSetup(renderTargets[target].geometryBatch);

            // for now, the default instanceBatch mesh is a primitive. This can be overwritten later.

           renderTargets[target].instanceBatch.mesh = UploadMesh(CreateRect({0,1},{1,1},{0,0},{1,0}));
            InstancedAttributeSetup(renderTargets[target].instanceBatch);
        }

        // sprite setup

        //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        //renderTargets[5].instanceBatch.texture = LoadTexture("fonts/Roboto/Roboto Bitmap.png");

        //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        //AttributeSetup(worldBatch);
        //AttributeSetup(rectBatch);
        //AttributeSetup(triBatch);

        // create shader program
        SPprimitiveWorld = CreateShaderProgram(
            "shaders/PrimitiveWorld.vert",
            "shaders/Primitive.frag"
        );

        SPprimitiveUI = CreateShaderProgram(
            "shaders/PrimitiveUI.vert",
            "shaders/Primitive.frag"
        );

        SPprimitiveSprite = CreateShaderProgram(
            "shaders/PrimitiveInstanceWorld.vert",
            "shaders/PrimitiveTexture.frag"
        );


        renderTargets[0].shaderProgram = SPprimitiveWorld;
        renderTargets[5].shaderProgram = SPprimitiveSprite;
        renderTargets[10].shaderProgram = SPprimitiveUI;
    }

     void Renderer::UploadUniforms(ShaderProgram& shader, const Camera& cam, const Screen& screen)
     {
        glUseProgram(shader.ID);

        for(Uniform u : shader.uniforms)
        {
            switch(u)
            {
                case Uniform::View:
                {
                    Mat3 view = Multiply(
                        Scale({
                            cam.zoom / cam.aspectRatio,
                            cam.zoom
                        }),
                        Translate({
                            -cam.position.x,
                            -cam.position.y
                        })
                    );

                    glUniformMatrix3fv(
                        shader.GetUniformLocation(Uniform::View),
                        1,
                        GL_TRUE,
                        &view.m[0][0]
                    );

                    break;
                }

                case Uniform::Screen:
                {
                    glUniform2f(
                        shader.GetUniformLocation(Uniform::Screen),
                        screen.dimensions.x,
                        screen.dimensions.y
                    );

                    break;
                }

                case Uniform::Texture0:
                {
                    glUniform1i(
                        shader.GetUniformLocation(Uniform::Texture0),
                        0
                    );

                    break;
                }
            }
        }
    }

    void Renderer::BeginFrame(const Camera& cam, const Screen& screen)
    {
        glClearColor(
            0.1f,
            0.2f,
            0.3f,
            1.0f
        );

        glClear(GL_COLOR_BUFFER_BIT);

        for(int target : renderOrder)
        {
            RenderTarget& t = renderTargets[target];

            UploadUniforms(t.program, cam, screen);
        }
    }

    void Renderer::Flush()
    {
        // set shader program and VBO
        //glUseProgram(shaderProgram);
        //glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);

        // draw batches
        //DrawGeometryBatch(worldBatch);
        //DrawInstanceBatch(rectBatch);
        //DrawInstanceBatch(triBatch);

        SortCommands();

        for(int target : renderOrder)
        {
            RenderTarget& t = renderTargets[target];

            glUseProgram(t.shaderProgram); // set shader per target

            if(t.geometryBatch.vertices.size() != 0)
            {
                DrawGeometryBatch(t.geometryBatch); // draw target's batch
            }
            if(t.instanceBatch.instances.size() != 0)
            {
                DrawInstanceBatch(t.instanceBatch);
            }
            

            t.geometryBatch.vertices.clear(); // clear batch data
            t.commands.clear(); // clear commands
            t.instanceBatch.instances.clear(); // clears instances

            //std::cout << t.commands.size() << ", " << t.geometryBatch.vertices.size() << "\n";
        }

        //DrawInstanceBatch(textBatch);

        //std::cout << textBatch.instances.size() << "\n";

        //textBatch.instances.clear();

        //std::cout << renderTargets.size() << "\n";

        //clear batches
        //worldBatch.vertices.clear();
        //rectBatch.instances.clear();
        //triBatch.instances.clear();
    }

    void Renderer::InstancedAttributeSetup(const InstanceBatch& batch)
    {
        // bind batch VAO
        glBindVertexArray(batch.mesh.VAO);

        // bind VBO
        glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);

        // position
        glEnableVertexAttribArray(3);
        glVertexAttribPointer(
            3,
            2,
            GL_FLOAT,
            GL_FALSE,
            sizeof(InstanceData),
            (void*)offsetof(InstanceData, transform.position)
        );
        glVertexAttribDivisor(3, 1);

        // scale
        glEnableVertexAttribArray(4);
        glVertexAttribPointer(
            4,
            2,
            GL_FLOAT,
            GL_FALSE,
            sizeof(InstanceData),
            (void*)offsetof(InstanceData, transform.scale)
        );
        glVertexAttribDivisor(4, 1);

        // rotation
        glEnableVertexAttribArray(5);
        glVertexAttribPointer(
            5,
            1,
            GL_FLOAT,
            GL_FALSE,
            sizeof(InstanceData),
            (void*)offsetof(InstanceData, transform.rotation)
        );
        glVertexAttribDivisor(5, 1);
        
        // color
        glEnableVertexAttribArray(6);
        glVertexAttribPointer(
            6,
            4,
            GL_FLOAT,
            GL_FALSE,
            sizeof(InstanceData),
            (void*)offsetof(InstanceData, color)
        );
        glVertexAttribDivisor(6, 1);

        // minUV
        glEnableVertexAttribArray(7);
        glVertexAttribPointer(
            7,
            2,
            GL_FLOAT,
            GL_FALSE,
            sizeof(InstanceData),
            (void*)offsetof(InstanceData, minUV)
        );
        glVertexAttribDivisor(7, 1);

        // maxUV
        glEnableVertexAttribArray(8);
        glVertexAttribPointer(
            8,
            2,
            GL_FLOAT,
            GL_FALSE,
            sizeof(InstanceData),
            (void*)offsetof(InstanceData, maxUV)
        );
        glVertexAttribDivisor(8, 1);
    }

    void Renderer::AttributeSetup(GeometryBatch& batch)
    {
        int MAX_VERTICES = 50000; // arbitrarily 50k

        glGenVertexArrays(1, &batch.VAO);
        glGenBuffers(1, &batch.VBO);

        glBindVertexArray(batch.VAO);
        glBindBuffer(GL_ARRAY_BUFFER, batch.VBO);

        // allocate empty dynamic buffer
        glBufferData(
            GL_ARRAY_BUFFER,
            sizeof(Vertex) * MAX_VERTICES,
            nullptr,
            GL_DYNAMIC_DRAW
        );

        // pos
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(
            0,
            2,
            GL_FLOAT,
            GL_FALSE,
            sizeof(Vertex),
            (void*)offsetof(Vertex, pos)
        );

        // col
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(
            1,
            4,
            GL_FLOAT,
            GL_FALSE,
            sizeof(Vertex),
            (void*)offsetof(Vertex, col)
        );

        // uv
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(
            2,
            2,
            GL_FLOAT,
            GL_FALSE,
            sizeof(Vertex),
            (void*)offsetof(Vertex, uv)
        );
    }

    void Renderer::DrawInstanceBatch(const InstanceBatch& batch)
    {
        //glUseProgram(SPprimitiveSprite); // this is also now going to assume the shader program is already set
            //despite this being potentially dangerous, it means less passing variables lol
            //hooray for a stateful system, I guess?
            //I mean I know for a fact this is only called in a singular place (as if 5/30/26)

        glBindVertexArray(batch.mesh.VAO);
        glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);

        glUniform1i(
            glGetUniformLocation(SPprimitiveSprite, "uTexture"),
            0 // why is this 0?
        );

        glActiveTexture(GL_TEXTURE0);

        glBindTexture(
            GL_TEXTURE_2D,
            batch.texture.ID
        );

        glBufferSubData(
            GL_ARRAY_BUFFER,
            0,
            batch.instances.size() * sizeof(InstanceData),
            batch.instances.data()
        );

        glDrawArraysInstanced(
            GL_TRIANGLES,
            0,
            batch.mesh.vertexCount,
            static_cast<GLsizei>(batch.instances.size())
        );
    }

    void Renderer::DrawGeometryBatch(const GeometryBatch& batch) // IMPORTANT: THIS ASSUMES SHADER PROGRAM IS ALREADY SET
    {
        glBindVertexArray(batch.VAO);
        glBindBuffer(GL_ARRAY_BUFFER, batch.VBO);

        glBufferData(
            GL_ARRAY_BUFFER,
            batch.vertices.size() * sizeof(Vertex),
            batch.vertices.data(),
            GL_DYNAMIC_DRAW
        );

        glDrawArrays(GL_TRIANGLES, 0, batch.vertices.size());

        //std::cout << batch.vertices.size() << "\n";
    }

    void Renderer::SetState(int target, int layer)
    {
        drawState.target = target;
        drawState.layer = layer;
    }

    void Renderer::SortCommands()
    {
        for(auto& pair : renderTargets)
        {
            RenderTarget& target = pair.second;

            // sort commands by layer
            std::stable_sort(
                target.commands.begin(),
                target.commands.end(),
                [](const RenderCommand& a, const RenderCommand& b)
                {
                    return a.layer < b.layer;
                }
            );

            size_t totalVertices = 0;

            for(const RenderCommand& cmd : target.commands)
            {
                std::visit([&](auto&& data)
                {
                    using T = std::decay_t<decltype(data)>;

                    if constexpr(std::is_same_v<T, DrawCommand>)
                    {
                        totalVertices += data.mesh.vertices.size();
                    }
                }, cmd.command);
            }

            //target.geometryBatch.vertices.clear();
            //target.instanceBatch.instances.clear();

            target.geometryBatch.vertices.reserve(totalVertices);

            for(const RenderCommand& cmd : target.commands)
            {
                std::visit([&](auto&& data)
                {
                    using T = std::decay_t<decltype(data)>;

                    if constexpr(std::is_same_v<T, DrawCommand>)
                    {
                        target.geometryBatch.vertices.insert(
                            target.geometryBatch.vertices.end(),
                            data.mesh.vertices.begin(),
                            data.mesh.vertices.end()
                        );
                    }
                    else if constexpr(std::is_same_v<T, InstanceData>)
                    {
                        target.instanceBatch.instances.push_back(data);
                    }
                }, cmd.command);
            }


        }
    }
}