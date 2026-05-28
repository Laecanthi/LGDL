#include <LGDL/Renderer.h>

#include <iostream>
#include <glad/glad.h>

#include <LGDL/Primitive.h>
#include <LGDL/Shader.h>
#include <LGDL/Math.h>

namespace LGDL
{
    void Renderer::Initialize()
    {
        //rectBatch.mesh = UploadMesh(CreateRectangleMesh()); // creates the rectangle mesh
        //triBatch.mesh = UploadMesh(CreateTriangleMesh()); // creates the triangle mesh

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glEnable(GL_MULTISAMPLE);

        int MAX_INSTANCES = 5000; // arbitrary max instances being 5k.
        int MAX_VERTICES = 50000; // arbitraryt max vertices being 50k.

        // create VBO
        glGenBuffers(1, &instanceVBO);
        glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(InstanceData) * MAX_INSTANCES, nullptr, GL_DYNAMIC_DRAW);

        // geometry batch setup
        glGenVertexArrays(1, &geometryBatch.VAO);
        glGenBuffers(1, &geometryBatch.VBO);

        glBindVertexArray(geometryBatch.VAO);
        glBindBuffer(GL_ARRAY_BUFFER, geometryBatch.VBO);

        // allocate empty dynamic buffer
        glBufferData(
            GL_ARRAY_BUFFER,
            sizeof(Vertex) * MAX_VERTICES,
            nullptr,
            GL_DYNAMIC_DRAW
        );

        // setup attributes
        AttributeSetup(geometryBatch);
        //AttributeSetup(rectBatch);
        //AttributeSetup(triBatch);

        // create shader program
        shaderProgram = CreateShaderProgram(
            "shaders/PrimitiveWorld.vert",
            "shaders/Primitive.frag"
        );
    }

    void Renderer::BeginFrame(const Camera& cam)
    {

        glUseProgram(shaderProgram);

        GLint viewLocation = glGetUniformLocation(shaderProgram, "uView");

        Mat3 view = LGDL::Multiply(
            LGDL::Scale({cam.zoom / cam.aspectRatio, cam.zoom}),
            LGDL::Translate({-cam.position.x, -cam.position.y})
        );

        glUniformMatrix3fv(viewLocation, 1, GL_FALSE, &view.m[0][0]);
    }

    void Renderer::Flush()
    {
        // set shader program and VBO
        glUseProgram(shaderProgram);
        glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);

        // draw batches
        DrawGeometryBatch(geometryBatch);
        //DrawInstanceBatch(rectBatch);
        //DrawInstanceBatch(triBatch);

        //clear batches
        geometryBatch.vertices.clear();
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

    void Renderer::AttributeSetup(const GeometryBatch& batch)
    {
        // bind batch VAO
        glBindVertexArray(batch.VAO);

        // bind VBO
        glBindBuffer(GL_ARRAY_BUFFER, batch.VBO);

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
    }
}