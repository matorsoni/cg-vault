#ifndef ARCBALL_HANDLER_HPP
#define ARCBALL_HANDLER_HPP

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

// ArcballHandler implements the Arcball interation from mouse input.
// Check https://en.wikibooks.org/wiki/OpenGL_Programming/Modern_OpenGL_Tutorial_Arcball
//
// Example of usage:

// // Setup Arcball handler.
// ArcballHandler arcball(window_width, window_height);
//
// // Main loop.
// while (!glfwWindowShouldClose(window))
// {
//      ...
//
//      // Process arcball motion.
//      arcball.processInput(window);
//
//      // Transform camera space rotation to world space rotation.
//      const mat4 arc_rotation = glm::inverse(camera.view()) * arcball.getArcRotation();
//
//      // Move the scene accordingly.
//      scene.ori_x = arc_rotation[0];
//      scene.ori_y = arc_rotation[1];
//      scene.ori_z = arc_rotation[2];
// }

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
