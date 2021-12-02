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
    Camera(float aspect_ratio);

    // Get position and orientation by reference.
    const glm::vec3& position() const;
    const glm::mat4& orientation() const;

    // Get transformations by reference.
    const glm::mat4& view() const;
    const glm::mat4& projection() const;

    // Update view transformation.
    // TODO: Maybe this should be a private method...
    void updateView();

    void setPosition(const glm::vec3& pos);
    // Move forwards, backwards, right, left.
    void moveF();
    void moveB();
    void moveR();
    void moveL();

    // Update camera orientation to follow the target.
    void lookAt(const glm::vec3& target);

    // Change projection type.
    void isPerspective(bool is_perspective);

private:

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

    // Moving speed.
    float speed_;
    // Vertical field of view.
    float fov_;
};

#endif // CAMERA_HPP
