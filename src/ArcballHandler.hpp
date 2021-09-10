#ifndef ARCBALL_HANDLER_HPP
#define ARCBALL_HANDLER_HPP

#include <GLFW/glfw3.h>
#include <glm/mat4x4.hpp>

// ArcballHandler implements the Arcball interation from mouse input.
class ArcballHandler
{
public:

    ArcballHandler(int screen_width, int screen_height);

    void processInput(GLFWwindow* window);

    const glm::mat4& getArcRotation() const;

private:

    void activate();
    void update();
    void deactivate();

    const int screen_width_;
    const int screen_height_;
    bool is_active_;
    int cursor_x_;
    int cursor_y_;
    glm::mat4 arc_rotation_;
};


#endif // ARCBALL_HANDLER_HPP
