#include <LGDL/Mesh.h>

#include <glad/glad.h>

namespace LGDL
{
    Mesh UploadMesh(const std::vector<Vertex>& vertices)
    {
        // create mesh

        Mesh mesh;

        glGenVertexArrays(1, &mesh.VAO);
        glGenBuffers(1, &mesh.VBO);

        glBindVertexArray(mesh.VAO);
        glBindBuffer(GL_ARRAY_BUFFER, mesh.VBO);

        glBufferData(
            GL_ARRAY_BUFFER,
            vertices.size() * sizeof(Vertex),
            vertices.data(),
            GL_STATIC_DRAW
        );

        // position
        glVertexAttribPointer(
            0, 2, GL_FLOAT, GL_FALSE,
            sizeof(Vertex),
            (void*)offsetof(Vertex, pos)
        );
        glEnableVertexAttribArray(0);

        // set vertex count
        mesh.vertexCount = (int)vertices.size();

        return mesh;
    }
}