#include <cmath>
#include <iostream>
#include <vector>

// OpenGL (via Glad) + GLFW includes.
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glad/glad.h>

#include "ArcballHandler.hpp"
#include "Camera.hpp"
#include "Math.hpp"
#include "Geometry.hpp"
#include "Renderer.hpp"
#include "Scene.hpp"
#include "SimpleGui.hpp"
#include "ShaderProgram.hpp"

using namespace std;
using glm::mat4;
using glm::vec2;
using glm::vec3;
using glm::vec4;

// GLFW error calback.
static void glfw_error_callback(int error, const char* description)
{
    fprintf(stderr, "Error: %s\n", description);
}

// Simple global place holder for variables changed by user input.
struct InputVariables
{
    float clip_plane_w = 100.0f;
};
InputVariables input;

void glfw_scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    static float factor = 0.05f;
    input.clip_plane_w -= static_cast<float>(yoffset) * factor;
}

// Directly process input with GLFW.
static void processInput(GLFWwindow* window, Camera& camera)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GLFW_TRUE);

    // Process camera movement.
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.moveF();
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.moveB();
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.moveR();
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.moveL();
}


// Main function.
int main()
{
    glfwSetErrorCallback(glfw_error_callback);

    if (!glfwInit()) {
        cout << "Failed to initialize GLFW." << endl;
        return -1;
    }

    const int window_width = 1280;
    const int window_height = 720;
    // Set minimum OpenGL version expected by the context.
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
    GLFWwindow* window = glfwCreateWindow(window_width, window_height, "Demo", NULL, NULL);
    if (!window) {
        cout << "Failed to create window." << endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    if (!gladLoadGLLoader( (GLADloadproc)glfwGetProcAddress) ) {
        cout << "Failed to initialize OpenGL context." << endl;
        glfwTerminate();
        return -1;
    }

    // Set GFLW callback functions.
    glfwSetScrollCallback(window, glfw_scroll_callback);

    // Print useful info.
    cout << "OpenGL version " << glGetString(GL_VERSION) << endl;
    cout << "GLSL version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << endl;
    cout << "OpenGL renderer: " << glGetString(GL_RENDERER) << endl;
    cout << "OpenGL vendor: " << glGetString(GL_VENDOR) << endl;

    // Set GLFW swap interval.
    glfwSwapInterval(1);
    // Set OpenGL constant states.
    glClearColor(0.0f, 0.15f, 0.15f, 1.0f);
    glEnable(GL_DEPTH_TEST);
    //glCullFace(GL_BACK);
    //glFrontFace(GL_CCW);
    //glEnable(GL_CULL_FACE);
    glViewport(0, 0, window_width, window_height);
    const float aspect_ratio = static_cast<float>(window_width) / window_height;

    TableSceneRenderer renderer(window_width, window_height);
    RenderParameter render_params;

    // Setup the scene.
    TableScene scene;

    // Setup camera.
    Camera camera(aspect_ratio);
    camera.setPosition(vec3(2.7f, 2.7f, 2.7f));
    camera.lookAt(vec3(0.0f, 1.1f, 0.0f));

    // Setup Arcball handler.
    ArcballHandler arcball(window_width, window_height);

    // Setup ImGui and GUI state.
    setupImGui(window);
    GuiState gui_state;

    // Main loop.
    double tick = glfwGetTime();
    double tock = 0.0;
    while (!glfwWindowShouldClose(window))
    {
        // Get time per frame (ms) and FPS.
        tock = glfwGetTime();
        auto time_per_frame = 1000.0 * (tock - tick);
        gui_state.time_per_frame = time_per_frame;
        tick = tock;

        glfwPollEvents();

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Create GUI frame.
        setupGuiFrame(gui_state);

        const vec3 gui_color = hsvToRgb(gui_state.H, gui_state.S, gui_state.V);
        const vec3 inv_color = hsvToRgb(gui_state.H > 180.f ? gui_state.H - 180.f : gui_state.H + 180.f,
                                        gui_state.S,
                                        gui_state.V);
        scene.sphere_material.kd = gui_color;
        scene.torus_material. kd = inv_color;

        // GLFW input handling.
        processInput(window, camera);
        camera.isPerspective(gui_state.is_perspective);

        // Update render parameters.
        render_params.ambient = gui_state.ambient;
        render_params.diffuse = gui_state.diffuse;
        render_params.specular = gui_state.specular;

        // Process arcball motion.
        arcball.processInput(window);
        // Transform camera space rotation to world space rotation.
        const mat4 arc_rotation = glm::inverse(camera.view()) * arcball.getArcRotation();
        // Move the scene accordingly.
        scene.root()->ori_x = arc_rotation[0];
        scene.root()->ori_y = arc_rotation[1];
        scene.root()->ori_z = arc_rotation[2];

        // World light position.
        scene.point_light_node->pos = vec3{4*cosf(tock), 6.2f, 4*sinf(tock)};

        // Update camera view before rendering.
        camera.updateView();

        // Render scene.
        renderer.renderTableScene(scene, camera, render_params);

        // Render GUI on top.
        renderGui();

        glfwSwapBuffers(window);
    }

    // ImGui cleanup.
    terminateImGui();

    // GLFW cleanup.
    glfwDestroyWindow(window);
    glfwTerminate();
}
