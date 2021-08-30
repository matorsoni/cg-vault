#ifndef GEOMETRY_HPP
#define GEOMETRY_HPP

#include <vector>

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

struct Vertex
{
public:
    Vertex();
    Vertex(const glm::vec3& p_pos);
    Vertex(const glm::vec3& p_pos,
           const glm::vec3& p_normal);
    Vertex(const glm::vec3& p_pos,
           const glm::vec3& p_normal,
           const glm::vec2& p_tex);
    glm::vec3 pos;
    glm::vec3 normal;
    glm::vec2 tex;
};

void createCube(std::vector<Vertex>& vertices);

void createCube(std::vector<Vertex>& vertices, std::vector<unsigned int>& indices);

void createSquare(std::vector<Vertex>& vertices, std::vector<unsigned int>& indices);

void createIcosahedron(std::vector<Vertex>& vertices, std::vector<unsigned int>& indices);

void createBezierPatch(std::vector<Vertex>& vertices, std::vector<unsigned int>& indices,
                       const std::vector<glm::vec3>& control_points, int rows, int cols, float sample_density = 1.0f);


#endif  // GEOMETRY_HPP
