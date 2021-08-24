#ifndef GEOMETRY_HPP
#define GEOMETRY_HPP

#include <vector>

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

struct Vertex
{
    glm::vec3 pos;
    glm::vec3 normal;
    glm::vec2 tex;
};

void createCube(std::vector<Vertex>& vertices);

void createCube(std::vector<Vertex>& vertices, std::vector<unsigned int>& indices);

void createSquare(std::vector<Vertex>& vertices, std::vector<unsigned int>& indices);


#endif  // GEOMETRY_HPP
