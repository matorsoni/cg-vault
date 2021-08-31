#ifndef TEAPOT_HPP
#define TEAPOT_HPP

#include <vector>

#include "Geometry.hpp"

void createTeapot(std::vector<Vertex>& vertices, std::vector<unsigned int>& indices, float density = 1.0f);

#endif // TEAPOT_HPP
