#include "ArcballHandler.hpp"

#include <cmath>
//#include <iostream>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/vector_angle.hpp>

using namespace std;
using glm::mat4;
using glm::vec3;

static vec3 spherePositionFromScreenPosition(double xpos, double ypos, int width, int height)
{
    // Convert to normalized display coordinates:
    // -1.0f <= x,y <= 1.0f
    float x = 2.0f * static_cast<float>(xpos / width) - 1.0f;
    float y = 1.0f - 2.0f * static_cast<float>(ypos / height);

    // Compute z if the point lies within the centered sphere with radius 1.
    float xy_radius2 = x*x + y*y;
    float z = xy_radius2 < 1.0f ? sqrt(1.0f - xy_radius2) : 0.0f;
    return vec3(x, y, z);
}

static mat4 rotationFromSpherePositions(const vec3& v1, const vec3& v2)
{
    const vec3 normal = glm::normalize(glm::cross(v1, v2));
    const float angle = glm::angle(v1, v2);

    mat4 rot{1.0f};
    rot = glm::rotate(rot, angle, normal);
    return rot;
}

// ArcballHandler implementation.
ArcballHandler::ArcballHandler(int screen_width, int screen_height):
    screen_width_(screen_width),
    screen_height_(screen_height),
    is_active_(false),
    sphere_pos_(vec3(0.0f, 0.0f, 1.0f)),
    arc_rotation_(mat4(1.0f))
{
}

void ArcballHandler::processInput(GLFWwindow* window)
{
    const int state = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT);
    if (state == GLFW_PRESS) {
        // Get mouse cursor position.
        double xpos, ypos;
        glfwGetCursorPos(window, &xpos, &ypos);

        // Save previous sphere position and update it.
        vec3 prev_sphere_pos = sphere_pos_;
        sphere_pos_ = spherePositionFromScreenPosition(xpos, ypos, screen_width_, screen_height_);
        //cout << sphere_pos_.x << " " << sphere_pos_.y <<  " " << sphere_pos_.z << endl;
        if (is_active_) {
            // Update transformation.
            arc_rotation_ = rotationFromSpherePositions(prev_sphere_pos, sphere_pos_);
        }
        else {
            // Activate.
            is_active_ = true;
        }
    }
    else {
        assert(state == GLFW_RELEASE);
        if (is_active_) {
            // Reset and deactivate.
            is_active_ = false;
            sphere_pos_ = vec3(0.0f, 0.0f, 1.0f);
            arc_rotation_ = mat4(1.0f);
        }
    }
}

const mat4& ArcballHandler::getArcRotation() const
{
    return arc_rotation_;
}
