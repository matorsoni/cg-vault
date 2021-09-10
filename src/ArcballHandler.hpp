#ifndef ARCBALL_HANDLER_HPP
#define ARCBALL_HANDLER_HPP

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

// ArcballHandler implements the Arcball interation from mouse input.
class ArcballHandler
{
public:

    ArcballHandler(int screen_width, int screen_height);

    void processInput(GLFWwindow* window);

    glm::mat4 getArcRotation() const;

private:

    const int screen_width_;
    const int screen_height_;
    bool is_active_;
    glm::vec3 sphere_pos_start_;
    // Current and last rotation matrices.
    glm::mat4 last_rotation_;
    glm::mat4 curr_rotation_;
};


#endif // ARCBALL_HANDLER_HPP
