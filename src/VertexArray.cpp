#include "VertexArray.hpp"

#include <glad/glad.h>

using namespace std;

VertexArray::VertexArray(const vector<Vertex>& p_vertices,
                         const vector<unsigned int>& p_indices):
    mesh(p_vertices, p_indices)
{
    // Create Vertex Array Object.
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    // Populate VAO with a Vertex Buffer Object.
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER,
                 mesh.vertices.size() * sizeof(Vertex),
                 mesh.vertices.data(),
                 GL_STATIC_DRAW);
    // Specify vertex positions, on layout location 0.
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // Specify vertex normals, on layout location 1.
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    // Specify vertex texture coords, on layout location 2.
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    // Setup Element Buffer Object if there are indices.
    if (mesh.indices.size() != 0) {
        glGenBuffers(1, &ebo);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                     mesh.indices.size() * sizeof(unsigned int),
                     mesh.indices.data(), GL_STATIC_DRAW);
    }

    // Unbind VAO.
    glBindVertexArray(0);
}

VertexArray::VertexArray(const Mesh& p_mesh):
    VertexArray(p_mesh.vertices, p_mesh.indices)
{
}

void VertexArray::draw()
{
    if (mesh.indices.empty()) {
        glDrawArrays(GL_TRIANGLES, 0, mesh.vertices.size());
    }
    else {
        glDrawElements(GL_TRIANGLES, mesh.indices.size(), GL_UNSIGNED_INT, (void*)0);
    }
}
