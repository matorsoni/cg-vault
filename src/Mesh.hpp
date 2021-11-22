#ifndef MESH_HPP
#define MESH_HPP

#include <vector>

#include "Vertex.hpp"

// Struct containing the basic geometric information of a 3D shape:
// A list of vertices and a list of indices representing its triangles.
class Mesh
{
public:
    Mesh();
    Mesh(const std::vector<Vertex>& p_vertices);
    Mesh(const std::vector<Vertex>& p_vertices,
         const std::vector<unsigned int>& p_indices);

    // Copy and append vertices and indices from another mesh.
    void extend(const Mesh& mesh);

    // Send data via OpenGL handles.
    void pushToGpu();

    void draw();

public:
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;

private:
    // Handles to OpenGL objects.
    unsigned int vao_;
    unsigned int vbo_;
    unsigned int ebo_;
};


#endif // MESH_HPP
