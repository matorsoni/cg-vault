#include "Geometry.hpp"

using namespace std;
using glm::vec2;
using glm::vec3;

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