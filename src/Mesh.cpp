#include "Mesh.hpp"

#include <glad/glad.h>

using namespace std;

Mesh::Mesh(const vector<Vertex>& p_vertices,
           const vector<unsigned int>& p_indices)
{
    // Copy vertices to the mesh object.
    vertices.reserve(p_vertices.size());
    for (const auto& v : p_vertices)
        vertices.emplace_back(v);

    // Create Vertex Array Object.
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    // Populate VAO with a Vertex Buffer Object.
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);
    // Specify vertex positions, on layout location 0.
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // Specify vertex texture coords, on layout location 2.
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // Setup Element Buffer Object if there are indices.
    if (p_indices.size() != 0) {
        assert(p_vertices.size() == p_indices.size());

        // Copy indices to the mesh object.
        indices.reserve(p_indices.size());
        for (const auto& ind : p_indices)
            indices.emplace_back(ind);

        // Create EBO.
        glGenBuffers(1, &ebo);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                     indices.size() * sizeof(unsigned int),
                     &indices[0], GL_STATIC_DRAW);
    }

    // Unbind VAO.
    glBindVertexArray(0);
}

void Mesh::draw()
{
    if (indices.empty()) {
        glDrawArrays(GL_TRIANGLES, 0, vertices.size());
    }
    else {
        glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_BYTE, (void*)0);
    }
}
