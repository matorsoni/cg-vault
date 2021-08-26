#include "Geometry.hpp"

#include <cmath>

using namespace std;
using glm::vec2;
using glm::vec3;

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

void createCube(vector<Vertex>& vertices)
{
    vertices = {
        // Position                 // Normal   // Texture
        {vec3(-0.5f, -0.5f, -0.5f), vec3(0.0f), vec2(0.0f, 0.0f)},
        {vec3( 0.5f, -0.5f, -0.5f), vec3(0.0f), vec2(1.0f, 0.0f)},
        {vec3( 0.5f,  0.5f, -0.5f), vec3(0.0f), vec2(1.0f, 1.0f)},
        {vec3( 0.5f,  0.5f, -0.5f), vec3(0.0f), vec2(1.0f, 1.0f)},
        {vec3(-0.5f,  0.5f, -0.5f), vec3(0.0f), vec2(0.0f, 1.0f)},
        {vec3(-0.5f, -0.5f, -0.5f), vec3(0.0f), vec2(0.0f, 0.0f)},

        {vec3(-0.5f, -0.5f,  0.5f), vec3(0.0f), vec2(0.0f, 0.0f)},
        {vec3( 0.5f, -0.5f,  0.5f), vec3(0.0f), vec2(1.0f, 0.0f)},
        {vec3( 0.5f,  0.5f,  0.5f), vec3(0.0f), vec2(1.0f, 1.0f)},
        {vec3( 0.5f,  0.5f,  0.5f), vec3(0.0f), vec2(1.0f, 1.0f)},
        {vec3(-0.5f,  0.5f,  0.5f), vec3(0.0f), vec2(0.0f, 1.0f)},
        {vec3(-0.5f, -0.5f,  0.5f), vec3(0.0f), vec2(0.0f, 0.0f)},

        {vec3(-0.5f,  0.5f,  0.5f), vec3(0.0f), vec2(1.0f, 0.0f)},
        {vec3(-0.5f,  0.5f, -0.5f), vec3(0.0f), vec2(1.0f, 1.0f)},
        {vec3(-0.5f, -0.5f, -0.5f), vec3(0.0f), vec2(0.0f, 1.0f)},
        {vec3(-0.5f, -0.5f, -0.5f), vec3(0.0f), vec2(0.0f, 1.0f)},
        {vec3(-0.5f, -0.5f,  0.5f), vec3(0.0f), vec2(0.0f, 0.0f)},
        {vec3(-0.5f,  0.5f,  0.5f), vec3(0.0f), vec2(1.0f, 0.0f)},

        {vec3( 0.5f,  0.5f,  0.5f), vec3(0.0f), vec2(1.0f, 0.0f)},
        {vec3( 0.5f,  0.5f, -0.5f), vec3(0.0f), vec2(1.0f, 1.0f)},
        {vec3( 0.5f, -0.5f, -0.5f), vec3(0.0f), vec2(0.0f, 1.0f)},
        {vec3( 0.5f, -0.5f, -0.5f), vec3(0.0f), vec2(0.0f, 1.0f)},
        {vec3( 0.5f, -0.5f,  0.5f), vec3(0.0f), vec2(0.0f, 0.0f)},
        {vec3( 0.5f,  0.5f,  0.5f), vec3(0.0f), vec2(1.0f, 0.0f)},

        {vec3(-0.5f, -0.5f, -0.5f), vec3(0.0f), vec2(0.0f, 1.0f)},
        {vec3( 0.5f, -0.5f, -0.5f), vec3(0.0f), vec2(1.0f, 1.0f)},
        {vec3( 0.5f, -0.5f,  0.5f), vec3(0.0f), vec2(1.0f, 0.0f)},
        {vec3( 0.5f, -0.5f,  0.5f), vec3(0.0f), vec2(1.0f, 0.0f)},
        {vec3(-0.5f, -0.5f,  0.5f), vec3(0.0f), vec2(0.0f, 0.0f)},
        {vec3(-0.5f, -0.5f, -0.5f), vec3(0.0f), vec2(0.0f, 1.0f)},

        {vec3(-0.5f,  0.5f, -0.5f), vec3(0.0f), vec2(0.0f, 1.0f)},
        {vec3( 0.5f,  0.5f, -0.5f), vec3(0.0f), vec2(1.0f, 1.0f)},
        {vec3( 0.5f,  0.5f,  0.5f), vec3(0.0f), vec2(1.0f, 0.0f)},
        {vec3( 0.5f,  0.5f,  0.5f), vec3(0.0f), vec2(1.0f, 0.0f)},
        {vec3(-0.5f,  0.5f,  0.5f), vec3(0.0f), vec2(0.0f, 0.0f)},
        {vec3(-0.5f,  0.5f, -0.5f), vec3(0.0f), vec2(0.0f, 1.0f)}
    };
}


void createSquare(std::vector<Vertex>& vertices, std::vector<unsigned int>& indices)
{
    vertices = {
        {vec3(-0.5f, 0.0f, 0.5f),  vec3(0.0f, 1.0f, 0.0f), vec2(0.0f, 0.0f)},
        {vec3(0.5f, 0.0f, 0.5f),   vec3(0.0f, 1.0f, 0.0f), vec2(1.0f, 0.0f)},
        {vec3(0.5f, 0.0f, -0.5f),  vec3(0.0f, 1.0f, 0.0f), vec2(1.0f, 1.0f)},
        {vec3(-0.5f, 0.0f, -0.5f), vec3(0.0f, 1.0f, 0.0f), vec2(0.0f, 1.0f)}
    };

    indices = {
        0, 1, 2,
        2, 3, 0
    };
}

void createIcosahedron(std::vector<Vertex>& vertices, std::vector<unsigned int>& indices)
{
    // Spherical coords with Y as up vector:
    // x = cos(phi)sin(theta)
    // y = sin(phi)
    // z = cos(phi)cos(theta)

    // Elevation from xz plane in degrees.
    // phi = arctan(0.5);
    const float PI = 3.14159f;
    const float phi = atanf(0.5f);
    const float d_theta = 2*PI/5;

    // Add top vertice.
    vertices.emplace_back(vec3(0.0f, 1.0f, 0.0f));

    // Add the 5 vertices below the first one.
    for (float theta = 0.0f; theta < 2*PI - 0.01f; theta += d_theta) {
        vec3 r{cosf(phi)*sinf(theta),
               sinf(phi),
               cosf(phi)*cosf(theta)};
        vertices.emplace_back(r, r);
    }

    // Add the next 5 vertices.
    for (float theta = 0.0f; theta < 2*PI - 0.01f; theta += d_theta) {
        vec3 r{cosf(-phi)*sinf(theta + d_theta/2),
               sinf(-phi),
               cosf(-phi)*cosf(theta + d_theta/2)};
        vertices.emplace_back(r, r);
    }

    // Add last vertice.
    vertices.emplace_back(vec3(0.0f, -1.0f, 0.0f));

    // Define triangles.
    indices = {
        // Top part.
        0,1,2,  0,2,3,  0,3,4,  0,4,5,  0,5,1,
        // Middle part.
        1,6,2,  2,6,7,  2,7,3,  3,7,8,  3,8,4,  4,8,9,  4,9,5,  5,9,10,  5,10,1,  1,10,6,
        // Bottom part.
        11,7,6,  11,8,7,  11,9,8,  11,10,9,  11,6,10
    };
}
