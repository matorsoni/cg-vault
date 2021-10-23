#include "ArcballHandler.hpp"

#include <algorithm>   // for std::min
#include <cmath>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/vector_angle.hpp>
#include <GLFW/glfw3.h>

#include "Math.hpp"   // for getRotation

using namespace std;
using glm::mat4;
using glm::vec3;

static vec3 spherePositionFromScreenPosition(double xpos, double ypos, int width, int height)
{
    // Convert to normalized display coordinates:
    // -1.0f <= x,y <= 1.0f
    vec3 p{
        2.0f * static_cast<float>(xpos / width) - 1.0f,
        1.0f - 2.0f * static_cast<float>(ypos / height),
        0.0f
    };

    // Check if the point lies within the centered sphere with radius 1.
    float xy_radius2 = p.x * p.x + p.y * p.y;
    if (xy_radius2 <= 1.0f)
        // Compute z coordinate on the sphere.
        p.z = sqrt(1.0f - xy_radius2);
    else
        // Nearest point on the sphere.
        p = glm::normalize(p);
    return p;
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

        if (is_active_) {
            // Update transformation.
            curr_rotation_ = rotationFromSpherePositions(sphere_pos_start_, sphere_pos);
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
