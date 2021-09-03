#ifndef MESH_HPP
#define MESH_HPP

#include <vector>

#include "Vertex.hpp"

struct Mesh
{
    Mesh() = default;
    Mesh(const std::vector<Vertex>& p_vertices);
    Mesh(const std::vector<Vertex>& p_vertices,
         const std::vector<unsigned int>& p_indices);

    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
};


#endif // MESH_HPP
