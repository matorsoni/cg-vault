#include "Camera.hpp"

#include <glm/glm.hpp>   // for glm::radians, glm::normalize, glm::cross...
#include <glm/gtc/matrix_transform.hpp>   // for glm::perspective

using glm::vec3;
using glm::vec4;
using glm::mat4;

vec3 Camera::UP_VECTOR = vec3(0.0f, 1.0f, 0.0f);

Camera::Camera(int screen_width, int screen_height):
    screen_height_(screen_height),
    screen_width_(screen_width),
    is_perspective_(true),
    position_(vec3(0.0f, 0.0f, 5.0f)),
    orientation_(mat4(1.0f))
{
    // Define perspective constants.
    const float aspect_ratio = static_cast<float>(screen_width_) / screen_height_;
    const float fov_y = glm::radians(45.0f);
    const float near_plane = 0.1f;
    const float far_plane = 100.0f;

    // Build projection matrices.
    perspective_projection_ = glm::perspective(fov_y, aspect_ratio, near_plane, far_plane);
}

vec3& Camera::position()
{
    return position_;
}

mat4& Camera::orientation()
{
    return orientation_;
}

const mat4& Camera::view()
{
    // View matrix is the inverse of:
    // | .  | .  | .  | .  | -1          | .  | Ex | .  | 0  |     | 1  | 0  | 0  | .  |
    // | Ex | Ey | Ez | P  |      =      | .  | Ey | .  | 0  |  *  | 0  | 1  | 0  | -P |
    // | .  | .  | .  | .  |             | .  | Ez | .  | 0  |     | 0  | 0  | 1  | .  |
    // | 0  | 0  | 0  | 1  |             | 0  | 0  | 0  | 1  |     | 0  | 0  | 0  | 1  |
    // P being the camera's position vector.
    // Ex, Ey and Ex being the X, Y, Z unit vectors of the camera's orthonormal frame.

    // Inverse translation matrix.
    mat4 inv_translation = mat4(1.0f);
    inv_translation[3] = vec4(-position_, 1.0f);

    // Update view.
    view_ = glm::transpose(orientation_) * inv_translation;

    return view_;
}

const mat4& Camera::projection()
{
    return is_perspective_ ? perspective_projection_ : parallel_projection_;
}

void Camera::lookAt(const vec3& target)
{
    vec3 Ez = glm::normalize(position_ - target);
    vec3 Ex = glm::normalize(glm::cross(UP_VECTOR, Ez));
    vec3 Ey = glm::cross(Ez, Ex);

    orientation_[0] = vec4(Ex, 0.0f);
    orientation_[1] = vec4(Ey, 0.0f);
    orientation_[2] = vec4(Ez, 0.0f);
    assert(orientation_[3] == vec4(0.0f, 0.0f, 0.0f, 1.0f));
}

void Camera::changeProjection()
{
    is_perspective_ = !is_perspective_;
}