#ifndef VERTEX_HPP
#define VERTEX_HPP

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

// Struct that contains every piece of data for a single vertex.
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

#endif // VERTEX_HPP
