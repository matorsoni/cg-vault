#include "Geometry.hpp"

#include <cmath>

#include "Math.hpp"

using namespace std;
using glm::vec2;
using glm::vec3;

// Generate triangle indices for a rectangular patch and append it to the input indicex list.
static void triangulatePatch(vector<unsigned int>& indices,
                             int rows,
                             int cols,
                             bool wrap_horizontally = false,
                             bool wrap_vertically = false,
                             unsigned int first_index = 0)
{
    // For each vertex position, append the corresponding right-hand oriented triangles.
    for (int i = 0; i < rows - 1; ++i) {
        for (int j = 0; j < cols - 1; ++j) {
            auto current = first_index + static_cast<unsigned int>(i*cols + j);
            auto right = current + 1;
            auto below = current + cols;
            auto right_below = current + cols + 1;

            // First triangle.
            indices.emplace_back(current);
            indices.emplace_back(below);
            indices.emplace_back(right);
            // Second triangle.
            indices.emplace_back(right);
            indices.emplace_back(below);
            indices.emplace_back(right_below);
        }

        if (wrap_horizontally) {
            // Glue right and left together.
            auto current = first_index + static_cast<unsigned int>(i*cols + cols-1);
            auto right = first_index + static_cast<unsigned int>(i*cols);
            auto below = current + cols;
            auto right_below = right + cols;
            // First triangle.
            indices.emplace_back(current);
            indices.emplace_back(below);
            indices.emplace_back(right);
            // Second triangle.
            indices.emplace_back(right);
            indices.emplace_back(below);
            indices.emplace_back(right_below);
        }
    }

    if (wrap_vertically) {
        // Glue top and bottom together.
        for (int j = 0; j < cols - 1; ++j) {
            auto current = first_index + static_cast<unsigned int>((rows-1)*cols + j);
            auto right = current + 1;
            auto below = first_index + j;
            auto right_below = below + 1;
            // First triangle.
            indices.emplace_back(current);
            indices.emplace_back(below);
            indices.emplace_back(right);
            // Second triangle.
            indices.emplace_back(right);
            indices.emplace_back(below);
            indices.emplace_back(right_below);
        }
    }
}

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


void createSquare(vector<Vertex>& vertices, vector<unsigned int>& indices)
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

void createIcosahedron(vector<Vertex>& vertices, vector<unsigned int>& indices)
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
    vertices.emplace_back(vec3(0.0f, 1.0f, 0.0f), vec3(0.0f, 1.0f, 0.0f));

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
    vertices.emplace_back(vec3(0.0f, -1.0f, 0.0f), vec3(0.0f, -1.0f, 0.0f));

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


void createBezierPatch(vector<Vertex>& vertices, vector<unsigned int>& indices,
                       const vector<vec3>& control_points, int rows, int cols, float sample_density)
{
    assert(rows >= 0);
    assert(cols >= 0);
    assert(sample_density >= 0.0f);
    assert(control_points.size() == rows * cols);

    // Compute row and col of resulting mesh.
    const auto row_samples = static_cast<int>(rows * sample_density);
    const auto col_samples = static_cast<int>(cols * sample_density);
    const float row_step = 1.0f / (row_samples - 1);
    const float col_step = 1.0f / (col_samples - 1);

    // Sample from Bezier surface.
    vertices.reserve(row_samples * col_samples);
    float u = 0.0f;
    float v = 0.0f;
    for (int i = 0; i < row_samples; ++i) {
        for (int j = 0; j < col_samples; ++j) {
            vertices.emplace_back(bezierSurfaceSample(control_points, rows, cols, u, v));
            v += col_step;
        }
        u += row_step;
        v = 0.0f;
    }

    // Triangulate the sampled patch.
    triangulatePatch(indices, rows, cols);
}
