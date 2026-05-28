#include <LGDL/Renderer.h>

#include <iostream>
#include <glad/glad.h>
#include <algorithm>

#include <LGDL/Primitive.h>
#include <LGDL/Shader.h>
#include <LGDL/Math.h>

namespace LGDL
{
    void Renderer::Initialize()
    {
        //rectBatch.mesh = UploadMesh(CreateRectangleMesh()); // creates the rectangle mesh
        //triBatch.mesh = UploadMesh(CreateTriangleMesh()); // creates the triangle mesh

        SetState(0,0);

        renderOrder.push_back(0); // world
        renderOrder.push_back(1); // ui

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glEnable(GL_MULTISAMPLE);

        int MAX_INSTANCES = 5000; // arbitrary max instances being 5k.
        int MAX_VERTICES = 50000; // arbitraryt max vertices being 50k.

        // create VBO
        glGenBuffers(1, &instanceVBO);
        glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(InstanceData) * MAX_INSTANCES, nullptr, GL_DYNAMIC_DRAW);

        // world batch setup


        // setup attributes
        for(int target : renderOrder)
        {
           AttributeSetup(renderTargets[target].geometryBatch);
        }
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


        renderTargets[0].shaderProgram = SPprimitiveWorld;
        renderTargets[1].shaderProgram = SPprimitiveUI;
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

        // SET PRIMITIVE WORLD ATTRIBUTES

        glUseProgram(SPprimitiveWorld);

        GLint viewLocation = glGetUniformLocation(SPprimitiveWorld, "uView");

        Mat3 view = Multiply(
            Scale({cam.zoom / cam.aspectRatio, cam.zoom}),
            Translate({-cam.position.x, -cam.position.y})
        );

        //std::cout << viewLocation << "\n";

        glUniformMatrix3fv(viewLocation, 1, GL_TRUE, &view.m[0][0]);

        // SET PRIMITIVE UI ATTRIBUTES

        glUseProgram(SPprimitiveUI);

        GLint screenLocation = glGetUniformLocation(SPprimitiveUI, "uScreen");

        glUniform2f(screenLocation, screen.dimensions.x, screen.dimensions.y);
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

            DrawGeometryBatch(t.geometryBatch); // draw target's batch

            t.geometryBatch.vertices.clear(); // clear batch data
            t.commands.clear(); // clear commands

            //std::cout << t.commands.size() << ", " << t.geometryBatch.vertices.size() << "\n";
        }

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
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(
            1,
            2,
            GL_FLOAT,
            GL_FALSE,
            sizeof(InstanceData),
            (void*)offsetof(InstanceData, transform.position)
        );
        glVertexAttribDivisor(1, 1);

        // scale
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(
            2,
            2,
            GL_FLOAT,
            GL_FALSE,
            sizeof(InstanceData),
            (void*)offsetof(InstanceData, transform.scale)
        );
        glVertexAttribDivisor(2, 1);

        // rotation
        glEnableVertexAttribArray(3);
        glVertexAttribPointer(
            3,
            1,
            GL_FLOAT,
            GL_FALSE,
            sizeof(InstanceData),
            (void*)offsetof(InstanceData, transform.rotation)
        );
        glVertexAttribDivisor(3, 1);


        
        // color
        glEnableVertexAttribArray(4);
        glVertexAttribPointer(
            4,
            4,
            GL_FLOAT,
            GL_FALSE,
            sizeof(InstanceData),
            (void*)offsetof(InstanceData, color)
        );
        glVertexAttribDivisor(4, 1);
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

    void Renderer::DrawInstanceBatch(const InstanceBatch& batch) // IMPORTANT: THIS ASSUMES VBO IS ALREADY BOUND AND SHADER PROGRAM IS SET
    {
        glBindVertexArray(batch.mesh.VAO);

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
                [](const DrawCommand& a, const DrawCommand& b)
                {
                    return a.layer < b.layer;
                }
            );

            // clear geometry buffer
            //target.geometryBatch.vertices.clear();
            //already clearned in flush(), doesn't need to ble cleared twice

            // pre-calculate total vertex count
            size_t totalVertices = 0;

            for(const DrawCommand& cmd : target.commands)
            {
                totalVertices += cmd.mesh.vertices.size();
            }

            target.geometryBatch.vertices.reserve(totalVertices);

            // flatten meshes into geometry buffer
            for(const DrawCommand& cmd : target.commands)
            {
                target.geometryBatch.vertices.insert(
                    target.geometryBatch.vertices.end(),
                    cmd.mesh.vertices.begin(),
                    cmd.mesh.vertices.end()
                );
            }
        }
    }
}