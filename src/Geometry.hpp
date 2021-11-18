#ifndef GEOMETRY_HPP
#define GEOMETRY_HPP

#include <vector>

#include <glm/vec3.hpp>

#include "Mesh.hpp"

void subdivide(Mesh& mesh, bool project_onto_unit_sphere = true);

Mesh createCubeWithoutIndices();

Mesh createCube();

Mesh createQuad();

Mesh createSquare();

Mesh createIcosahedron();

Mesh createSubdividedIcosahedron(int order);

Mesh createTorus(float radius_a, float radius_b);

Mesh createBezierPatch(const std::vector<glm::vec3>& control_points,
                       int rows,
                       int cols,
                       float sample_density = 1.0f);


#endif  // GEOMETRY_HPP
