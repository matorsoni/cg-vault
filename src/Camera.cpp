#include "Camera.hpp"

#include <glm/glm.hpp>   // for glm::radians, glm::normalize, glm::cross...
#include <glm/gtc/matrix_transform.hpp>   // for glm::perspective

using glm::vec3;
using glm::vec4;
using glm::mat4;

vec3 Camera::UP_VECTOR = vec3(0.0f, 1.0f, 0.0f);

Camera::Camera(float aspect_ratio):
    is_perspective_(true),
    position_(vec3(0.0f, 0.0f, 5.0f)),
    orientation_(mat4(1.0f)),
    speed_(0.1f),
    fov_(glm::radians(45.0f))
{
    assert(aspect_ratio > 0.0f);

    // Define perspective constants.
    const float near_plane = 0.1f;
    const float far_plane = 100.0f;
    const float ortho_factor = 2.0f;

    // Build projection matrices.
    perspective_projection_ = glm::perspective(fov_, aspect_ratio, near_plane, far_plane);
    parallel_projection_ = glm::ortho(-aspect_ratio * ortho_factor, aspect_ratio * ortho_factor,
                                      -ortho_factor, ortho_factor,
                                      near_plane, far_plane);
}

const vec3& Camera::position() const
{
    return position_;
}

const mat4& Camera::orientation() const
{
    return orientation_;
}

const mat4& Camera::view() const
{
    return view_;
}

const mat4& Camera::projection() const
{
    return is_perspective_ ? perspective_projection_ : parallel_projection_;
}

void Camera::updateView()
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
}

void Camera::setPosition(const glm::vec3& pos)
{
    position_ = pos;
}

void Camera::moveF()
{
    position_ -= speed_ * vec3(orientation_[2]);
}

void Camera::moveB()
{
    position_ += speed_ * vec3(orientation_[2]);
}

void Camera::moveR()
{
    position_ += speed_ * vec3(orientation_[0]);
}

void Camera::moveL()
{
    position_ -= speed_ * vec3(orientation_[0]);
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

void Camera::isPerspective(bool is_perspective)
{
    is_perspective_ = is_perspective;
}
