#include <cmath>
#include <iostream>
#include <vector>

// OpenGL (via Glad) + GLFW includes.
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>

#include "ArcballHandler.hpp"
#include "Camera.hpp"
#include "Math.hpp"
#include "Geometry.hpp"
#include "SceneNode.hpp"
#include "SimpleGui.hpp"
#include "ShaderProgram.hpp"
#include "Teapot.hpp"
#include "VertexArray.hpp"

using namespace std;
using glm::mat4;
using glm::vec2;
using glm::vec3;
using glm::vec4;

// GLFW error calback.
static void error_callback(int error, const char* description)
{
    fprintf(stderr, "Error: %s\n", description);
}

// Process input with GLFW.
static void processInput(GLFWwindow* window, Camera& camera)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GLFW_TRUE);

    // Process camera inputs...
}


// Main function.
int main()
{
    glfwSetErrorCallback(error_callback);

    if (!glfwInit()) {
        cout << "Failed to initialize GLFW." << endl;
        return -1;
    }

    const int window_width = 960;
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

    // Print useful info.
    cout << "OpenGL version " << glGetString(GL_VERSION) << endl;
    cout << "GLSL version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << endl;
    cout << "OpenGL renderer: " << glGetString(GL_RENDERER) << endl;
    cout << "OpenGL vendor: " << glGetString(GL_VENDOR) << endl;

    // Set GLFW swap interval.
    glfwSwapInterval(1);
    // Set OpenGL constant states.
    glClearColor(0.0f, 0.2f, 0.2f, 1.0f);
    glEnable(GL_DEPTH_TEST);
    //glCullFace(GL_BACK);
    //glFrontFace(GL_CCW);
    //glEnable(GL_CULL_FACE);
    glViewport(0, 0, window_width, window_height);
    const float aspect_ratio = static_cast<float>(window_width) / window_height;

    // Setup ImGui and GUI state.
    setupImGui(window);
    GuiState gui_state;

    // Create cube VertexArray.
    VertexArray cube(createCubeWithoutIndices());

    // Create square VertexArray.
    VertexArray square(createSquare());

    // Icosahedron.
    Mesh ico_mesh = createIcosahedron();
    const int subdivision_order = 3;
    for (int i = 0; i < subdivision_order; ++i) subdivide(ico_mesh);
    VertexArray icosahedron(ico_mesh);

    // Create Teapot VertexArray.
    const float sample_density = 2.0f;
    VertexArray teapot(createTeapot(sample_density));

    ShaderProgram shader_normal("../src/shader/VertexColorFromNormal.vert",
                                "../src/shader/VertexColor.frag");

    /*** Setup the scene. ***/
    SceneNode scene;
    // Table variables.
    const float table_width = 2.0f;
    const float table_depth = 1.0f;
    const float table_height = 1.0f;
    const float leg_width = 0.1f;
    const float top_girth = 0.1f;
    const float top_scale_factor = 1.1f;
    // Table object.
    SceneNode* table_object = scene.makeSubnode();
    // Legs.
    {
        const vec3 leg_scale = vec3(leg_width, table_height, leg_width);
        vec3 leg_position[] = {
            vec3(table_depth/2, table_height/2, table_width/2),
            vec3(table_depth/2, table_height/2, -table_width/2),
            vec3(-table_depth/2, table_height/2, -table_width/2),
            vec3(-table_depth/2, table_height/2, table_width/2)
        };
        for (int i = 0; i < 4; ++i) {
            SceneNode* leg_object = table_object->makeSubnode();
            leg_object->pos = leg_position[i];
            leg_object->scale = leg_scale;
            leg_object->vertex_array = &cube;
        }
    }
    // Top.
    {
        SceneNode* top_object = table_object->makeSubnode();
        top_object->pos = vec3(0.0f, table_height + top_girth/2, 0.0f);
        top_object->scale = vec3(top_scale_factor * table_depth,
                                 top_girth,
                                 top_scale_factor * table_width);
        top_object->vertex_array = &cube;
    }

    const float table_top_y = table_height + top_girth;

    // Icosahedron object.
    SceneNode* ico_object = scene.makeSubnode();
    ico_object->pos = vec3{0.0f, table_top_y + 0.8f, -0.5f};
    ico_object->scale = vec3(0.35f);
    ico_object->vertex_array = &icosahedron;

    // Teapot object.
    SceneNode* teapot_object = scene.makeSubnode();
    teapot_object->pos = vec3(0.0f, table_top_y + 0.01f, 0.65f);
    teapot_object->ori_y = vec3(0.0f, 0.0f, -1.0f);
    teapot_object->ori_z = cross(teapot_object->ori_x, teapot_object->ori_y);
    teapot_object->scale = vec3(0.2f);
    teapot_object->vertex_array = &teapot;

    // Setup camera.
    Camera camera(window_width, window_height);
    camera.position() = vec3(2.7f, 2.7f, 2.7f);
    camera.lookAt(vec3(0.0f, table_top_y, 0.0f));

    // Setup Arcball handler.
    ArcballHandler arcball(window_width, window_height);

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

        // Create GUI frame and render it.
        setupGuiFrame(gui_state);
        renderGui();

        // GLFW input handling.
        processInput(window, camera);
        camera.isPerspective(gui_state.is_perspective);

        // Create camera view-projection matrix transform.
        mat4 view_projection = camera.projection() * camera.view();

        // Process arcball motion.
        arcball.processInput(window);
        // Transform camera space rotation to world space rotation.
        const mat4 arc_rotation = glm::inverse(camera.view()) * arcball.getArcRotation();
        // Move the scene accordingly.
        scene.ori_x = arc_rotation[0];
        scene.ori_y = arc_rotation[1];
        scene.ori_z = arc_rotation[2];

        shader_normal.use();
        shader_normal.setUniformMat4f("u_view_projection", glm::value_ptr(view_projection));

        // Draw table.
        for (int i = 0; i < table_object->subnodes.size(); ++i) {
            auto* node = table_object->subnodes[i].get();
            auto model = node->worldTransformation();
            shader_normal.setUniformMat4f("u_model", glm::value_ptr(model));
            glBindVertexArray(node->vertex_array->vao);
            node->vertex_array->draw();
        }

        // Draw icosahedron.
        {
            auto model = ico_object->worldTransformation();
            shader_normal.setUniformMat4f("u_model", glm::value_ptr(model));
            glBindVertexArray(ico_object->vertex_array->vao);
            // Draw wireframe and then turn back to fill triangles.
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
            ico_object->vertex_array->draw();
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        }

        // Draw teapot.
        {
            auto model = teapot_object->worldTransformation();
            shader_normal.setUniformMat4f("u_model", glm::value_ptr(model));
            glBindVertexArray(teapot_object->vertex_array->vao);
            teapot_object->vertex_array->draw();
        }

        glfwSwapBuffers(window);
    }

    // ImGui cleanup.
    terminateImGui();

    // GLFW cleanup.
    glfwDestroyWindow(window);
    glfwTerminate();
}
