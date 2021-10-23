#ifndef VERTEX_ARRAY_HPP
#define VERTEX_ARRAY_HPP

#include <vector>

#include "Mesh.hpp"

// VertexArray is a wrapper for vertex array objects in OpenGL.
struct VertexArray
{
    VertexArray(const std::vector<Vertex>& p_vertices,
                const std::vector<unsigned int>& p_indices);
    VertexArray(const Mesh& p_mesh);

    void draw();

    unsigned int vao;
    unsigned int vbo;
    unsigned int ebo;
    Mesh mesh;
};


#endif // VERTEX_ARRAY_HPP
