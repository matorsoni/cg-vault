#ifndef GEOMETRY_HPP
#define GEOMETRY_HPP

#include <vector>

#include "Vertex.hpp"

void createCube(std::vector<Vertex>& vertices);

void createCube(std::vector<Vertex>& vertices, std::vector<unsigned int>& indices);

void createSquare(std::vector<Vertex>& vertices, std::vector<unsigned int>& indices);

void createIcosahedron(std::vector<Vertex>& vertices, std::vector<unsigned int>& indices);

void createBezierPatch(std::vector<Vertex>& vertices, std::vector<unsigned int>& indices,
                       const std::vector<glm::vec3>& control_points, int rows, int cols, float sample_density = 1.0f);


#endif  // GEOMETRY_HPP
