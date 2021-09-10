#include "Math.hpp"

#include <cmath>

using glm::vec3;
using glm::vec4;
using glm::mat4;
using glm::cross;
using glm::normalize;
using glm::radians;

using namespace std;

mat4 getRotationX(float angle)
{
    const float s = sin(angle);
    const float c = cos(angle);
    // GLM defines matrices as (col0x, col0y, col0z, col0w, ...), always remeber it's transposed.
    return mat4(
        1, 0, 0, 0,
        0, c, s, 0,
        0, -s, c, 0,
        0, 0, 0, 1
    );
}

mat4 getRotationY(float angle)
{
    const float s = sin(angle);
    const float c = cos(angle);
    // GLM defines matrices as (col0x, col0y, col0z, col0w, ...), always remeber it's transposed.
    return mat4(
        c, 0, -s, 0,
        0, 1, 0, 0,
        s, 0, c, 0,
        0, 0, 0, 1
    );
}

mat4 getRotationZ(float angle)
{
    const float s = sin(angle);
    const float c = cos(angle);
    // GLM defines matrices as (col0x, col0y, col0z, col0w, ...), always remeber it's transposed.
    return mat4(
        c, s, 0, 0,
       -s, c, 0, 0,
        0, 0, 1, 0,
        0, 0, 0, 1
    );
}

mat4 getRotation(const vec3& ori_x,
                 const vec3& ori_y,
                 const vec3& ori_z)
{
    return mat4(vec4(glm::normalize(ori_x), 0.0f),
                vec4(glm::normalize(ori_y), 0.0f),
                vec4(glm::normalize(ori_z), 0.0f),
                vec4(0.0f, 0.0f, 0.0f, 1.0f));
}

// Ref: http://courses.cms.caltech.edu/cs171/assignments/hw3/hw3-notes/notes-hw3.html
mat4 getRotation(const vec3& axis, float angle)
{
    const float x = axis.x;
    const float y = axis.y;
    const float z = axis.z;
    const float c = cosf(angle);
    const float s = sinf(angle);

    const float x2 = x * x;
    const float y2 = y * y;
    const float z2 = z * z;
    const float xy = x * y;
    const float xz = x * z;
    const float yz = y * z;

    const float inv_cos = 1.0f - c;

    return mat4(
        vec4(x2 + (1.f-x2)*c, xy*inv_cos + z*s, xz*inv_cos - y*s, 0.0f),
        vec4(xy*inv_cos - z*s, y2 + (1.f-y2)*c, yz*inv_cos + x*s, 0.0f),
        vec4(xz*inv_cos + y*s, yz*inv_cos - x*s, z2 + (1.f-z2)*c, 0.0f),
        vec4(0.0f, 0.0f, 0.0f, 1.0f)
    );
}

mat4 getScale(const vec3& u)
{
    return mat4(
        u.x, 0, 0, 0,
        0, u.y, 0, 0,
        0, 0, u.z, 0,
        0, 0, 0, 1
    );
}


mat4 getTranslation(const vec3& u)
{
    // Initialize as identity.
    mat4 trans(1.0);
    // Set the fourth column to be the translation vector.
    trans[3] = vec4(u, 1.0);
    return trans;
}


mat4 getView(const vec3& camera_pos, const vec3& up, const vec3& target)
{
    // View matrix is the inverse of:
    // | .  | .  | .  | .  | -1          | .  | Ex | .  | 0  |     | 1  | 0  | 0  | .  |
    // | Ex | Ey | Ez | P  |      =      | .  | Ey | .  | 0  |  *  | 0  | 1  | 0  | -P |
    // | .  | .  | .  | .  |             | .  | Ez | .  | 0  |     | 0  | 0  | 1  | .  |
    // | 0  | 0  | 0  | 1  |             | 0  | 0  | 0  | 1  |     | 0  | 0  | 0  | 1  |
    // Ex, Ey and Ex being the X, Y, Z unit vectors of the camera's frame.

    vec3 Ez = normalize(camera_pos - target);
    vec3 Ex = normalize(cross(up, Ez));
    vec3 Ey = cross(Ez, Ex);

    mat4 inv_rotation = transpose(
        mat4{vec4(Ex, 0.0f), vec4(Ey, 0.0f), vec4(Ez, 0.0f), vec4(0.0f, 0.0f, 0.0f, 1.0f)}
    );
    mat4 inv_translation = mat4(1.0f);
    inv_translation[3] = vec4(-camera_pos, 1.0f);

    return inv_rotation * inv_translation;
}


uint64_t factorial(int n)
{
    assert(n >= 0);

    if (n == 0)
        return 1;

    uint64_t result = 1;
    while (n > 0) {
        result *= static_cast<uint64_t>(n);
        n--;
    }

    return result;
}

// Bernstein polynomial.
float bernstein(int n, int i, float x)
{
    // Allow some imprecision.
    assert(x >= 0.0f);
    assert(x <= 1.001f);

    if (n < 0 || i < 0 || i > n) {
        return 0.0f;
    }

    if (n == 0) {
        return 1.0f;
    }

    float binomial = static_cast<float>(factorial(n) / (factorial(i) * factorial(n - i)));
    return binomial * powf(x, i) * powf(1.0f-x, n-i);
}

// Derivative of the Bernstein polynomial.
float d_bernstein(int n, int i, float x)
{
    assert (n >= 0);
    return (bernstein(n-1, i-1, x) - bernstein(n-1, i, x)) * n;
}

// Compute the position and normal vectors at surface point (u,v).
tuple<vec3, vec3> bezierSurfaceSample(const vector<vec3>& control_points,
                                      int rows,
                                      int cols,
                                      float u,
                                      float v)
{
    // Position, derivatives of position with respect to u and v.
    vec3 position{0.0f};
    vec3 du_position{0.0f};
    vec3 dv_position{0.0f};

    // Compute sums on the Bezier surface, following this notation:
    // https://en.wikipedia.org/wiki/B%C3%A9zier_surface
    for (int i = 0; i < rows; ++i) {
        vec3 sum_v{0.0f};
        vec3 sum_dv{0.0f};
        const float bern_i = bernstein(rows-1, i, u);
        const float d_bern_i = d_bernstein(rows-1, i, u);

        for (int j = 0; j < cols; ++j) {
            const float bern_j = bernstein(cols-1, j, v);
            const float d_bern_j = d_bernstein(cols-1, j, v);
            const vec3 k_ij = control_points[i*cols + j];

            sum_v += bern_j * k_ij;
            sum_dv += d_bern_j * k_ij;
        }

        position += bern_i * sum_v;
        du_position += d_bern_i * sum_v;
        dv_position += bern_i * sum_dv;
    }

    // Compute normalized normal.
    vec3 normal = normalize(cross(du_position, dv_position));

    return make_tuple(position, normal);
}
