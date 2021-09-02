#ifndef VERTEX_ARRAY_HPP
#define VERTEX_ARRAY_HPP

#include <vector>

#include "Geometry.hpp"

struct VertexArray
{
    VertexArray(const std::vector<Vertex>& p_vertices,
                const std::vector<unsigned int>& p_indices);

    void draw();

    unsigned int vao;
    unsigned int vbo;
    unsigned int ebo;
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
};


#endif // VERTEX_ARRAY_HPP
