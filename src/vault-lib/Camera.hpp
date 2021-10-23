#ifndef CAMERA_HPP
#define CAMERA_HPP

#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>

class Camera
{
public:

    // Global camera up vector.
    static glm::vec3 UP_VECTOR;

    Camera() = delete;
    Camera(int screen_width, int screen_height);

    // Get position and orientation by reference.
    glm::vec3& position();
    glm::mat4& orientation();

    // Update transformations and return by reference.
    const glm::mat4& view();
    const glm::mat4& projection();

    // Update camera orientation to follow the target.
    void lookAt(const glm::vec3& target);

    // Change projection type.
    void isPerspective(bool is_perspective);
    void changeProjection();

private:

    // Screen resolution.
    int screen_width_;
    int screen_height_;

    // Projection is either perspective or parallel.
    bool is_perspective_;

    // Position and orientation in global coordinates.
    // Orientation is the following matrix, with normalized Ex, Ey and Ez.
    // | .  | .  | .  | 0  |
    // | Ex | Ey | Ez | 0  |
    // | .  | .  | .  | 0  |
    // | 0  | 0  | 0  | 1  |
    glm::vec3 position_;
    glm::mat4 orientation_;

    // Transformations.
    glm::mat4 view_;
    glm::mat4 perspective_projection_;
    glm::mat4 parallel_projection_;
};

#endif // CAMERA_HPP
