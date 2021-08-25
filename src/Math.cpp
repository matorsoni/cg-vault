#include "Math.hpp"

using namespace glm;


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
