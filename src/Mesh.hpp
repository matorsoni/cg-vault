#ifndef MESH_HPP
#define MESH_HPP

#include <vector>

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

struct Vertex
{
    glm::vec3 pos;
    glm::vec3 normal;
    glm::vec2 tex;
};

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
