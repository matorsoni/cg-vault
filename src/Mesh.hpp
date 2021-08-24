#ifndef MESH_HPP
#define MESH_HPP

#include <vector>

#include "Geometry.hpp"

struct Mesh
{
    Mesh(const std::vector<Vertex>& p_vertices,
         const std::vector<unsigned int>& p_indices);

    void draw();

    unsigned int vao;
    unsigned int vbo;
    unsigned int ebo;
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
};


#endif // MESH_HPP
