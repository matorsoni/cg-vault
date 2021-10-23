#include "Vertex.hpp"

using glm::vec2;
using glm::vec3;

// Vertex intialization.
Vertex::Vertex():
    pos(vec3(0.0f)), normal(vec3(0.0f)), tex(vec2(0.0f))
{
}

Vertex::Vertex(const vec3& p_pos):
    pos(p_pos), normal(vec3(0.0f)), tex(vec2(0.0f))
{
}

Vertex::Vertex(const vec3& p_pos, const vec3& p_normal):
    pos(p_pos), normal(p_normal), tex(vec2(0.0f))
{
}

Vertex::Vertex(const vec3& p_pos, const vec3& p_normal, const vec2& p_tex):
    pos(p_pos), normal(p_normal), tex(p_tex)
{
}
