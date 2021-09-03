#ifndef GEOMETRY_HPP
#define GEOMETRY_HPP

#include <vector>

#include <glm/vec3.hpp>

#include "Mesh.hpp"

Mesh createCubeWithoutIndices();

Mesh createCube();

Mesh createSquare();

Mesh createIcosahedron();

void createBezierPatch(Mesh& mesh,
                       const std::vector<glm::vec3>& control_points,
                       int rows,
                       int cols,
                       float sample_density = 1.0f);


#endif  // GEOMETRY_HPP
