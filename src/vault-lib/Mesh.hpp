#ifndef MESH_HPP
#define MESH_HPP

#include <vector>

#include "Vertex.hpp"

// Struct containing the basic geometric information of a 3D shape:
// A list of vertices and a list of indices representing its triangles.
struct Mesh
{
public:
    Mesh() = default;
    Mesh(const std::vector<Vertex>& p_vertices);
    Mesh(const std::vector<Vertex>& p_vertices,
         const std::vector<unsigned int>& p_indices);

    // Copy and append vertices and indices from another mesh.
    void extend(const Mesh& mesh);

public:
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
};


#endif // MESH_HPP
