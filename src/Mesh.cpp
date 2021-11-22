#include "Mesh.hpp"

#include <glad/glad.h>

using namespace std;

Mesh::Mesh(const vector<Vertex>& p_vertices,
           const vector<unsigned int>& p_indices):
    vao_{0}, vbo_{0}, ebo_{0}
{
    // Copy vertices to the Mesh object.
    if (!p_vertices.empty()) {
        vertices.reserve(p_vertices.size());
        for (const auto& v : p_vertices)
            vertices.emplace_back(v);
    }

    // Copy indices to the Mesh object.
    if (!p_indices.empty()) {
        indices.reserve(p_indices.size());
        for (const auto& ind : p_indices)
            indices.emplace_back(ind);
    }

    // Create OpenGL objects.
    glGenVertexArrays(1, &vao_);
    glGenBuffers(1, &vbo_);
    glGenBuffers(1, &ebo_);
}

Mesh::Mesh(const vector<Vertex>& p_vertices):
    Mesh(p_vertices, {})
{
}

Mesh::Mesh():
    Mesh({}, {})
{
}

void Mesh::extend(const Mesh& mesh)
{
    if (mesh.indices.empty()) {
        return;
    }

    const unsigned int initial_vertex_count = vertices.size();

    for (const auto& v : mesh.vertices) {
        vertices.emplace_back(v);
    }

    for (const auto& ind : mesh.indices) {
        indices.emplace_back(initial_vertex_count + ind);
    }
}

void Mesh::pushToGpu()
{
    assert(vao_ != 0);
    assert(vbo_ != 0);
    assert(ebo_ != 0);
    assert(!vertices.empty());

    glBindVertexArray(vao_);

    // Populate Vertex Buffer Object with vertex data.
    glBindBuffer(GL_ARRAY_BUFFER, vbo_);
    glBufferData(GL_ARRAY_BUFFER,
                 vertices.size() * sizeof(Vertex),
                 vertices.data(),
                 GL_STATIC_DRAW);

    // Specify vertex positions, on layout location 0.
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    // Specify vertex normals, on layout location 1.
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    // Specify vertex texture coords, on layout location 2.
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));

    // Setup Element Buffer Object if there are indices.
    if (!indices.empty()) {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo_);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                     indices.size() * sizeof(unsigned int),
                     indices.data(),
                     GL_STATIC_DRAW);
    }

    // Unbind.
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void Mesh::draw()
{
    glBindVertexArray(vao_);

    if (indices.empty()) {
        glDrawArrays(GL_TRIANGLES, 0, vertices.size());
    }
    else {
        glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, (void*)0);
    }
}
