#include "ArcballHandler.hpp"

using glm::mat4;

ArcballHandler::ArcballHandler(int screen_width, int screen_height):
    screen_width_(screen_width),
    screen_height_(screen_height),
    is_active_(false),
    cursor_x_(0),
    cursor_y_(0),
    arc_rotation_(mat4(1.0f))
{
}

void ArcballHandler::processInput(GLFWwindow* window)
{
    int state = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT);
    if (state == GLFW_PRESS) {
        if (is_active_) {
            update();
        }
        else {
            activate();
        }
    }
    else {
        assert(state == GLFW_RELEASE);
        if (is_active_) {
            deactivate();
        }
    }

    double xpos, ypos;
    glfwGetCursorPos(window, &xpos, &ypos);
    //cout << xpos << " " << ypos << endl;


}

const mat4& ArcballHandler::getArcRotation() const
{
    return arc_rotation_;
}

void ArcballHandler::activate()
{

}

void ArcballHandler::update()
{

}

void ArcballHandler::deactivate()
{

}
