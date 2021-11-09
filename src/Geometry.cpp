#include "Geometry.hpp"

#include <cmath>
#include <tuple>   // For std::tuple

#include <glm/glm.hpp>

#include "Math.hpp"

using namespace std;
using glm::vec2;
using glm::vec3;
using glm::normalize;

static constexpr float PI = 3.14159f;

// Generate triangle indices for a rectangular patch and append it to the input indicex list.
//
// . . . .
// . . . .   ->   |\|\|\|
// . . . .        |\|\|\|
//
// Order of vertices: a triangle's last vertex (OpenGL's provoking vertex by default) is the same
// between neighbouring triangles. This makes for a pleasant flat shading.
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
            auto current     = first_index + static_cast<unsigned int>(i*cols + j);
            auto right       = current + 1;
            auto below       = current + cols;
            auto right_below = current + cols + 1;

            // First triangle.
            indices.emplace_back(below);
            indices.emplace_back(right_below);
            indices.emplace_back(current);
            // Second triangle.
            indices.emplace_back(right_below);
            indices.emplace_back(right);
            indices.emplace_back(current);
        }

        if (wrap_horizontally) {
            // Glue right and left together.
            auto current     = first_index + static_cast<unsigned int>(i*cols + cols-1);
            auto right       = first_index + static_cast<unsigned int>(i*cols);
            auto below       = current + cols;
            auto right_below = right + cols;
            // First triangle.
            indices.emplace_back(below);
            indices.emplace_back(right_below);
            indices.emplace_back(current);
            // Second triangle.
            indices.emplace_back(right_below);
            indices.emplace_back(right);
            indices.emplace_back(current);
        }
    }

    if (wrap_vertically) {
        // Glue top and bottom together.
        for (int j = 0; j < cols - 1; ++j) {
            auto current     = first_index + static_cast<unsigned int>((rows-1)*cols + j);
            auto right       = current + 1;
            auto below       = first_index + j;
            auto right_below = below + 1;
            // First triangle.
            indices.emplace_back(below);
            indices.emplace_back(right_below);
            indices.emplace_back(current);
            // Second triangle.
            indices.emplace_back(right_below);
            indices.emplace_back(right);
            indices.emplace_back(current);
        }

        // Treat the wrapping of the very last triagle.
        if (wrap_horizontally) {
            auto current     = first_index + static_cast<unsigned int>((rows-1)*cols + cols-1);
            auto right       = first_index + static_cast<unsigned int>((rows-1)*cols);
            auto below       = first_index + static_cast<unsigned int>(cols-1);
            auto right_below = first_index;
            // First triangle.
            indices.emplace_back(below);
            indices.emplace_back(right_below);
            indices.emplace_back(current);
            // Second triangle.
            indices.emplace_back(right_below);
            indices.emplace_back(right);
            indices.emplace_back(current);
        }
    }
}


// Subdivide each triangle in a mesh.
void subdivide(Mesh& mesh, bool project_onto_unit_sphere)
{
    assert(!mesh.indices.empty());
    assert(mesh.indices.size() % 3 == 0);

    const auto initial_index_count = mesh.indices.size();

    vector<unsigned int> new_indices;
    new_indices.reserve(4 * initial_index_count);

    for (int tri_index_slot = 0; tri_index_slot < initial_index_count; tri_index_slot += 3) {
        // Retrieve triangle data.
        const unsigned int index[3] = {
            mesh.indices.at(tri_index_slot),
            mesh.indices.at(tri_index_slot + 1),
            mesh.indices.at(tri_index_slot + 2)
        };
        const vec3 tri_pos[3] = {
            mesh.vertices.at(index[0]).pos,
            mesh.vertices.at(index[1]).pos,
            mesh.vertices.at(index[2]).pos
        };

        // Compute mean points between vertices.
        vec3 new_pos[3] = {
            (tri_pos[0] + tri_pos[1]) * 0.5f,
            (tri_pos[1] + tri_pos[2]) * 0.5f,
            (tri_pos[2] + tri_pos[0]) * 0.5f
        };

        unsigned int previous_vertex_count = mesh.vertices.size();

        if (project_onto_unit_sphere) {
            for (auto& p : new_pos) {
                p = normalize(p);
                mesh.vertices.emplace_back(p, p);
            }
        }
        else {
            for (const auto& p : new_pos) {
                mesh.vertices.emplace_back(p);
            }
        }

        // Define new triangles.
        const unsigned int new_pos_index[3] = {
            previous_vertex_count,
            previous_vertex_count + 1,
            previous_vertex_count + 2
        };

        // Top triangle.
        new_indices.push_back(index[0]);
        new_indices.push_back(new_pos_index[0]);
        new_indices.push_back(new_pos_index[2]);
        // Left triangle.
        new_indices.push_back(new_pos_index[0]);
        new_indices.push_back(index[1]);
        new_indices.push_back(new_pos_index[1]);
        // Right triangle.
        new_indices.push_back(new_pos_index[2]);
        new_indices.push_back(new_pos_index[1]);
        new_indices.push_back(index[2]);
        // Middle triangle.
        new_indices.push_back(new_pos_index[0]);
        new_indices.push_back(new_pos_index[1]);
        new_indices.push_back(new_pos_index[2]);
    }

    // Copy new indices to the mesh.
    mesh.indices.clear();
    mesh.indices.reserve(new_indices.size());
    for (const auto& ind : new_indices) {
        mesh.indices.push_back(ind);
    }
}


Mesh createCubeWithoutIndices()
{
    vector<Vertex> vertices = {
        // Position                 // Normal                // Texture
        {vec3(-0.5f, -0.5f, -0.5f), vec3(0.0f, 0.0f, -1.0f), vec2(0.0f, 0.0f)},
        {vec3( 0.5f, -0.5f, -0.5f), vec3(0.0f, 0.0f, -1.0f), vec2(1.0f, 0.0f)},
        {vec3( 0.5f,  0.5f, -0.5f), vec3(0.0f, 0.0f, -1.0f), vec2(1.0f, 1.0f)},
        {vec3( 0.5f,  0.5f, -0.5f), vec3(0.0f, 0.0f, -1.0f), vec2(1.0f, 1.0f)},
        {vec3(-0.5f,  0.5f, -0.5f), vec3(0.0f, 0.0f, -1.0f), vec2(0.0f, 1.0f)},
        {vec3(-0.5f, -0.5f, -0.5f), vec3(0.0f, 0.0f, -1.0f), vec2(0.0f, 0.0f)},

        {vec3(-0.5f, -0.5f,  0.5f), vec3(0.0f, 0.0f, 1.0f), vec2(0.0f, 0.0f)},
        {vec3( 0.5f, -0.5f,  0.5f), vec3(0.0f, 0.0f, 1.0f), vec2(1.0f, 0.0f)},
        {vec3( 0.5f,  0.5f,  0.5f), vec3(0.0f, 0.0f, 1.0f), vec2(1.0f, 1.0f)},
        {vec3( 0.5f,  0.5f,  0.5f), vec3(0.0f, 0.0f, 1.0f), vec2(1.0f, 1.0f)},
        {vec3(-0.5f,  0.5f,  0.5f), vec3(0.0f, 0.0f, 1.0f), vec2(0.0f, 1.0f)},
        {vec3(-0.5f, -0.5f,  0.5f), vec3(0.0f, 0.0f, 1.0f), vec2(0.0f, 0.0f)},

        {vec3(-0.5f,  0.5f,  0.5f), vec3(-1.0f, 0.0f, 0.0f), vec2(1.0f, 0.0f)},
        {vec3(-0.5f,  0.5f, -0.5f), vec3(-1.0f, 0.0f, 0.0f), vec2(1.0f, 1.0f)},
        {vec3(-0.5f, -0.5f, -0.5f), vec3(-1.0f, 0.0f, 0.0f), vec2(0.0f, 1.0f)},
        {vec3(-0.5f, -0.5f, -0.5f), vec3(-1.0f, 0.0f, 0.0f), vec2(0.0f, 1.0f)},
        {vec3(-0.5f, -0.5f,  0.5f), vec3(-1.0f, 0.0f, 0.0f), vec2(0.0f, 0.0f)},
        {vec3(-0.5f,  0.5f,  0.5f), vec3(-1.0f, 0.0f, 0.0f), vec2(1.0f, 0.0f)},

        {vec3( 0.5f,  0.5f,  0.5f), vec3(1.0f, 0.0f, 0.0f), vec2(1.0f, 0.0f)},
        {vec3( 0.5f,  0.5f, -0.5f), vec3(1.0f, 0.0f, 0.0f), vec2(1.0f, 1.0f)},
        {vec3( 0.5f, -0.5f, -0.5f), vec3(1.0f, 0.0f, 0.0f), vec2(0.0f, 1.0f)},
        {vec3( 0.5f, -0.5f, -0.5f), vec3(1.0f, 0.0f, 0.0f), vec2(0.0f, 1.0f)},
        {vec3( 0.5f, -0.5f,  0.5f), vec3(1.0f, 0.0f, 0.0f), vec2(0.0f, 0.0f)},
        {vec3( 0.5f,  0.5f,  0.5f), vec3(1.0f, 0.0f, 0.0f), vec2(1.0f, 0.0f)},

        {vec3(-0.5f, -0.5f, -0.5f), vec3(0.0f, -1.0f, 0.0f), vec2(0.0f, 1.0f)},
        {vec3( 0.5f, -0.5f, -0.5f), vec3(0.0f, -1.0f, 0.0f), vec2(1.0f, 1.0f)},
        {vec3( 0.5f, -0.5f,  0.5f), vec3(0.0f, -1.0f, 0.0f), vec2(1.0f, 0.0f)},
        {vec3( 0.5f, -0.5f,  0.5f), vec3(0.0f, -1.0f, 0.0f), vec2(1.0f, 0.0f)},
        {vec3(-0.5f, -0.5f,  0.5f), vec3(0.0f, -1.0f, 0.0f), vec2(0.0f, 0.0f)},
        {vec3(-0.5f, -0.5f, -0.5f), vec3(0.0f, -1.0f, 0.0f), vec2(0.0f, 1.0f)},

        {vec3(-0.5f,  0.5f, -0.5f), vec3(0.0f, 1.0f, 0.0f), vec2(0.0f, 1.0f)},
        {vec3( 0.5f,  0.5f, -0.5f), vec3(0.0f, 1.0f, 0.0f), vec2(1.0f, 1.0f)},
        {vec3( 0.5f,  0.5f,  0.5f), vec3(0.0f, 1.0f, 0.0f), vec2(1.0f, 0.0f)},
        {vec3( 0.5f,  0.5f,  0.5f), vec3(0.0f, 1.0f, 0.0f), vec2(1.0f, 0.0f)},
        {vec3(-0.5f,  0.5f,  0.5f), vec3(0.0f, 1.0f, 0.0f), vec2(0.0f, 0.0f)},
        {vec3(-0.5f,  0.5f, -0.5f), vec3(0.0f, 1.0f, 0.0f), vec2(0.0f, 1.0f)}
    };

    return Mesh(vertices);
}


Mesh createQuad()
{
    vector<Vertex> vertices = {
        {vec3(-1.0f,  1.0f, 0.0f), vec3(0.0f), vec2(0.0f, 1.0f)},
        {vec3(-1.0f, -1.0f, 0.0f), vec3(0.0f), vec2(0.0f, 0.0f)},
        {vec3( 1.0f, -1.0f, 0.0f), vec3(0.0f), vec2(1.0f, 0.0f)},
        {vec3( 1.0f,  1.0f, 0.0f), vec3(0.0f), vec2(1.0f, 1.0f)}
    };

    vector<unsigned int> indices = {
        0, 1, 2,
        2, 3, 0
    };

    return Mesh(vertices, indices);
}

Mesh createSquare()
{
    vector<Vertex> vertices = {
        {vec3(-0.5f, 0.0f, 0.5f),  vec3(0.0f, 1.0f, 0.0f), vec2(0.0f, 0.0f)},
        {vec3(0.5f, 0.0f, 0.5f),   vec3(0.0f, 1.0f, 0.0f), vec2(1.0f, 0.0f)},
        {vec3(0.5f, 0.0f, -0.5f),  vec3(0.0f, 1.0f, 0.0f), vec2(1.0f, 1.0f)},
        {vec3(-0.5f, 0.0f, -0.5f), vec3(0.0f, 1.0f, 0.0f), vec2(0.0f, 1.0f)}
    };

    vector<unsigned int> indices = {
        0, 1, 2,
        2, 3, 0
    };

    return Mesh(vertices, indices);
}

Mesh createIcosahedron()
{
    // Spherical coords with Y as up vector:
    // x = cos(phi)sin(theta)
    // y = sin(phi)
    // z = cos(phi)cos(theta)

    // Elevation from xz plane in degrees.
    // phi = arctan(0.5);
    const float phi = atanf(0.5f);
    const float d_theta = 2*PI/5;

    // Add top vertice.
    vector<Vertex> vertices;
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
    vector<unsigned int> indices = {
        // Top part.
        0,1,2,  0,2,3,  0,3,4,  0,4,5,  0,5,1,
        // Middle part.
        1,6,2,  2,6,7,  2,7,3,  3,7,8,  3,8,4,  4,8,9,  4,9,5,  5,9,10,  5,10,1,  1,10,6,
        // Bottom part.
        11,7,6,  11,8,7,  11,9,8,  11,10,9,  11,6,10
    };

    return Mesh(vertices, indices);
}


Mesh createTorus(float radius_a, float radius_b)
{
    assert(radius_a >= 0.0f);
    assert(radius_b >= 0.0f);

    const int num_samples_u = 30;
    const int num_samples_v = 20;
    const float step_u = 1.0f / num_samples_u;
    const float step_v = 1.0f / num_samples_v;

    vector<Vertex> vertices;
    for (int j = 0; j < num_samples_v; ++j) {
        for (int i = 0; i < num_samples_u; ++i) {
            const float theta = i * step_u * 2 * PI;
            const float phi   = j * step_v * 2 * PI;
            const float cos_theta = cosf(theta);
            const float sin_theta = sin(theta);
            const float cos_phi   = cosf(phi);
            const float sin_phi   = sinf(phi);

            // Compute torus position.
            const float pos_x = (radius_a + radius_b * cos_phi) * sin_theta;
            const float pos_y = radius_b * sin_phi;
            const float pos_z = (radius_a + radius_b * cos_phi) * cos_theta;

            // Compute normal vector.
            const float n_x = cos_phi * sin_theta;
            const float n_y = sin_phi;
            const float n_z = cos_phi * cos_theta;

            vertices.emplace_back(vec3{pos_x, pos_y, pos_z}, vec3{n_x, n_y, n_z});
        }
    }

    vector<unsigned int> indices;
    triangulatePatch(indices, num_samples_v, num_samples_u, true, true);

    return Mesh(vertices, indices);
}


Mesh createBezierPatch(const vector<vec3>& control_points,
                       int rows,
                       int cols,
                       float sample_density)
{
    assert(rows >= 0);
    assert(cols >= 0);
    assert(sample_density >= 0.0f);
    assert(control_points.size() == rows * cols);

    Mesh mesh;

    // Compute row and col of resulting mesh.
    const auto row_samples = static_cast<int>(rows * sample_density);
    const auto col_samples = static_cast<int>(cols * sample_density);
    const float row_step = 1.0f / (row_samples - 1);
    const float col_step = 1.0f / (col_samples - 1);

    // Sample from Bezier surface.
    float u = 0.0f;
    float v = 0.0f;
    for (int i = 0; i < row_samples; ++i) {
        for (int j = 0; j < col_samples; ++j) {
            const auto sample = bezierSurfaceSample(control_points, rows, cols, u, v);
            const vec3 position = get<0>(sample);
            const vec3 normal = get<1>(sample);
            mesh.vertices.emplace_back(position, normal);

            v += col_step;
        }
        u += row_step;
        v = 0.0f;
    }

    // Triangulate the sampled patch.
    triangulatePatch(mesh.indices, row_samples, col_samples);

    return mesh;
}
