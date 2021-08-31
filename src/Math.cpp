#include "Math.hpp"

#include <cmath>

using glm::vec3;
using glm::vec4;
using glm::mat4;
using glm::radians;

using namespace std;

mat4 getRotationX(float angle)
{
    float s = sin(radians(angle));
    float c = cos(radians(angle));
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
    float s = sin(radians(angle));
    float c = cos(radians(angle));
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
    float s = sin(radians(angle));
    float c = cos(radians(angle));
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

float bernstein(int n, int i, float x)
{
    assert(n >= 0);
    assert(i >= 0);
    assert(i <= n);
    assert(x >= 0.0f);
    assert(x <= 1.0f);

    float binomial = static_cast<float>(factorial(n) / (factorial(i) * factorial(n - i)));
    return binomial * powf(x, i) * powf(1.0f-x, n-i);
}

vec3 bezierSurfaceSample(const vector<vec3>& control_points, int rows, int cols, float u, float v)
{
    vec3 point{0.0f};
    for (int i = 0; i < rows; ++i) {
        float bern_i = bernstein(rows-1, i, u);
        vec3 aux_point{0.0f};
        for (int j = 0; j < cols; ++j) {
            float bern_j = bernstein(cols-1, j, v);
            aux_point += bern_j * control_points[i*cols + j];
        }

        point += bern_i * aux_point;
    }

    return point;
}
