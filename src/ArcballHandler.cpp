#include "ArcballHandler.hpp"

#include <algorithm>   // for std::min
#include <cmath>
//#include <iostream>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/vector_angle.hpp>

#include "Math.hpp"   // for getRotation

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
    // v1 and v2 are supposed to be normalized.
    // Compute normal vector that is orthogonal to v1 and v2.
    vec3 normal = glm::cross(v1, v2);
    if (glm::length(normal) < 0.0001f) {
        return mat4(1.0f);
    }
    normal = glm::normalize(normal);

    // Angle between v1 and v2, in radians. Use min to prevent float imprecisions (ex: 1.0001f).
    const float angle = acos(min(1.0f, glm::dot(v1, v2)));

    return getRotation(normal, angle);
}

// ArcballHandler implementation.
ArcballHandler::ArcballHandler(int screen_width, int screen_height):
    screen_width_(screen_width),
    screen_height_(screen_height),
    is_active_(false),
    sphere_pos_start_(vec3(0.0f, 0.0f, 1.0f)),
    last_rotation_(mat4(1.0f)),
    curr_rotation_(mat4(1.0f))
{
}

void ArcballHandler::processInput(GLFWwindow* window)
{
    const int state = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT);
    if (state == GLFW_PRESS) {
        // Get mouse cursor position.
        double xpos, ypos;
        glfwGetCursorPos(window, &xpos, &ypos);

        // Compute current sphere position.
        vec3 sphere_pos = spherePositionFromScreenPosition(xpos,
                                                           ypos,
                                                           screen_width_,
                                                           screen_height_);
        //cout << sphere_pos_start_.x << " " << sphere_pos_start_.y <<  " " << sphere_pos_start_.z << endl;
        if (is_active_) {
            // Update transformation.
            curr_rotation_ = rotationFromSpherePositions(sphere_pos, sphere_pos_start_);
        }
        else {
            // Activate and save initial sphere position.
            is_active_ = true;
            sphere_pos_start_ = sphere_pos;
        }
    }
    else {
        assert(state == GLFW_RELEASE);
        if (is_active_) {
            // Reset and deactivate.
            is_active_ = false;
            sphere_pos_start_ = vec3(0.0f, 0.0f, 1.0f);
            last_rotation_ = curr_rotation_ * last_rotation_;
            curr_rotation_ = mat4(1.0f);
        }
    }
}

mat4 ArcballHandler::getArcRotation() const
{
    return curr_rotation_ * last_rotation_;
}
