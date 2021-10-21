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
    static const float camera_speed = 0.1f;
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.position() -= camera_speed * vec3(camera.orientation()[2]);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.position() += camera_speed * vec3(camera.orientation()[2]);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.position() += camera_speed * vec3(camera.orientation()[0]);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.position() -= camera_speed * vec3(camera.orientation()[0]);
}

// Convertion from HSV to RGB.
// Inputs:
// - H in range [0, 360.0];
// - S in range [0, 1.0];
// - H in range [0, 1.0].
// Output: RGB color in range [0, 1.0].
static vec3 hsvToRgb(float h, float s, float v)
{
    assert(h >= 0.0f);
    assert(h <= 360.0f);
    assert(s >= 0.0f);
    assert(s <= 1.0f);
    assert(v >= 0.0f);
    assert(v <= 1.0f);

    static const vec3 BLACK    {0.0f, 0.0f, 0.0f};
    static const vec3 WHITE    {1.0f, 1.0f, 1.0f};
    static const vec3 RED      {1.0f, 0.0f, 0.0f};
    static const vec3 YELLOW   {1.0f, 1.0f, 0.0f};
    static const vec3 GREEN    {0.0f, 1.0f, 0.0f};
    static const vec3 CYAN     {0.0f, 1.0f, 1.0f};
    static const vec3 BLUE     {0.0f, 0.0f, 1.0f};
    static const vec3 MAGENTA  {1.0f, 0.0f, 1.0f};
    static const vec3 HUE_WHEEL[6] = {RED, YELLOW, GREEN, CYAN, BLUE, MAGENTA};

    float int_part_float = 0.0f;
    // Division by 60 + 0.01 to make sure the integer part can not reach 6.
    const float frac_part = modf(h / 60.01f, &int_part_float);
    const auto int_part = static_cast<int>(int_part_float);
    assert(frac_part >= 0.0f);
    assert(frac_part <= 1.0f);
    assert(int_part >= 0);
    assert(int_part <= 5);

    // Determine the color hue.
    vec3 color_a, color_b;
    if (int_part >= 5) {
        color_a = HUE_WHEEL[5];
        color_b = HUE_WHEEL[0];
    }
    else {
        color_a = HUE_WHEEL[int_part];
        color_b = HUE_WHEEL[int_part + 1];
    }
    vec3 final_color = Lerp(color_a, color_b, frac_part);

    // Factor "saturation" in.
    final_color = Lerp(WHITE, final_color, s);
    // Factor "value" in.
    final_color = Lerp(BLACK, final_color, v);

    return final_color;
}

struct PhongMaterial
{
    vec3 ka{0.0f, 0.0f, 0.0f};
    vec3 kd{0.0f, 1.0f, 0.0f};
    vec3 ks{1.0f, 1.0f, 1.0f};
    float shiny{10.0f};
};

// Main function.
int main()
{
    glfwSetErrorCallback(error_callback);

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
    glEnable(GL_CLIP_PLANE0);
    //glCullFace(GL_BACK);
    //glFrontFace(GL_CCW);
    //glEnable(GL_CULL_FACE);
    glViewport(0, 0, window_width, window_height);
    const float aspect_ratio = static_cast<float>(window_width) / window_height;

    // Create cube VertexArray.
    VertexArray cube(createCubeWithoutIndices());

    // Create square VertexArray.
    VertexArray square(createSquare());

    // Create icosahedron VertexArray.
    Mesh ico_mesh = createIcosahedron();
    const int subdivision_order = 3;
    for (int i = 0; i < subdivision_order; ++i) subdivide(ico_mesh);
    VertexArray icosahedron(ico_mesh);

    // Create torus VertexArray.
    Mesh torus_mesh = createTorus(1.0f, 0.15f);
    VertexArray torus(torus_mesh);

    // Create Teapot VertexArray.
    const float sample_density = 2.0f;
    VertexArray teapot(createTeapot(sample_density));

    //ShaderProgram shader_normal("../src/shader/ClippingPlane.vert",
    //                            "../src/shader/VertexColor.frag");
    //ShaderProgram shader_single_color("../src/shader/ClippingPlaneSingleColor.vert",
    //                                  "../src/shader/VertexColor.frag");
    ShaderProgram shader_gouraud("../src/shader/GouraudSingleColor.vert",
                                 "../src/shader/VertexColor.frag");
    ShaderProgram shader_phong("../src/shader/Phong.vert",
                               "../src/shader/Phong.frag");

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

    // Torus object.
    SceneNode* torus_object = scene.makeSubnode();
    torus_object->pos = ico_object->pos;
    torus_object->scale = vec3(0.5f);
    torus_object->vertex_array = &torus;

    // Teapot object.
    SceneNode* teapot_object = scene.makeSubnode();
    teapot_object->pos = vec3(0.0f, table_top_y + 0.01f, 0.65f);
    teapot_object->ori_y = vec3(0.0f, 0.0f, -1.0f);
    teapot_object->ori_z = cross(teapot_object->ori_x, teapot_object->ori_y);
    teapot_object->scale = vec3(0.2f);
    teapot_object->vertex_array = &teapot;

    // Materials.
    PhongMaterial ico_material;
    PhongMaterial torus_material;
    PhongMaterial table_material;
        table_material.kd = vec3(1.0f, 1.0f, 0.0f);
    PhongMaterial teapot_material;
        teapot_material.kd = vec3(0.0f, 0.0f, 1.0f);

    // Setup camera.
    Camera camera(window_width, window_height);
    camera.position() = vec3(2.7f, 2.7f, 2.7f);
    camera.lookAt(vec3(0.0f, table_top_y, 0.0f));

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
        ico_material  .kd = gui_color;
        torus_material.kd = inv_color;

        // GLFW input handling.
        processInput(window, camera);
        camera.isPerspective(gui_state.is_perspective);

        // Process arcball motion.
        arcball.processInput(window);
        // Transform camera space rotation to world space rotation.
        const mat4 arc_rotation = glm::inverse(camera.view()) * arcball.getArcRotation();
        // Move the scene accordingly.
        scene.ori_x = arc_rotation[0];
        scene.ori_y = arc_rotation[1];
        scene.ori_z = arc_rotation[2];

        // Determine shader to be used.
        ShaderProgram& shader = gui_state.shader == 0 ? shader_gouraud :
                                gui_state.shader == 1 ? shader_phong   :
                                shader_gouraud;

        shader.use();
        shader.setUniformMat4f("u_view", glm::value_ptr(camera.view()));
        shader.setUniformMat4f("u_projection", glm::value_ptr(camera.projection()));

        // Light direction.
        vec3 light_direction{-cosf(tock), -sinf(tock), 0.0f};
        shader.setUniform3f("u_light_direction",
                            light_direction.x,
                            light_direction.y,
                            light_direction.z);

        // Update lighting parameters from GUI.
        shader.setUniform1f("u_ambient_coef", gui_state.ambient);
        shader.setUniform1f("u_diffuse_coef", gui_state.diffuse);
        shader.setUniform1f("u_specular_coef", gui_state.specular);

        // Define clipping plane.
        vec4 plane{-1.0f, -1.0f, -1.0f, input.clip_plane_w};
        shader.setUniform4f("u_clip_plane", plane.x, plane.y, plane.z, plane.w);

        // Draw table.
        for (int i = 0; i < table_object->subnodes.size(); ++i) {
            auto* node = table_object->subnodes[i].get();
            auto model = node->worldTransformation();
            shader.setUniformVec3f("u_ka", glm::value_ptr(table_material.ka));
            shader.setUniformVec3f("u_kd", glm::value_ptr(table_material.kd));
            shader.setUniformVec3f("u_ks", glm::value_ptr(table_material.ks));
            shader.setUniform1f("u_shiny", table_material.shiny);
            shader.setUniformMat4f("u_model", glm::value_ptr(model));
            glBindVertexArray(node->vertex_array->vao);
            node->vertex_array->draw();
        }

        // Draw torus.
        {
            auto model = torus_object->worldTransformation();
            shader.setUniformVec3f("u_ka", glm::value_ptr(torus_material.ka));
            shader.setUniformVec3f("u_kd", glm::value_ptr(torus_material.kd));
            shader.setUniformVec3f("u_ks", glm::value_ptr(torus_material.ks));
            shader.setUniform1f("u_shiny", torus_material.shiny);
            shader.setUniformMat4f("u_model", glm::value_ptr(model));
            glBindVertexArray(torus_object->vertex_array->vao);
            torus_object->vertex_array->draw();
        }

        // Draw teapot.
        {
            auto model = teapot_object->worldTransformation();
            shader.setUniformVec3f("u_ka", glm::value_ptr(teapot_material.ka));
            shader.setUniformVec3f("u_kd", glm::value_ptr(teapot_material.kd));
            shader.setUniformVec3f("u_ks", glm::value_ptr(teapot_material.ks));
            shader.setUniform1f("u_shiny", teapot_material.shiny);
            shader.setUniformMat4f("u_model", glm::value_ptr(model));
            glBindVertexArray(teapot_object->vertex_array->vao);
            teapot_object->vertex_array->draw();
        }

        // Draw icosahedron.
        {
            shader.setUniformVec3f("u_ka", glm::value_ptr(ico_material.ka));
            shader.setUniformVec3f("u_kd", glm::value_ptr(ico_material.kd));
            shader.setUniformVec3f("u_ks", glm::value_ptr(ico_material.ks));
            shader.setUniform1f("u_shiny", ico_material.shiny);
            auto model = ico_object->worldTransformation();
            shader.setUniformMat4f("u_model", glm::value_ptr(model));
            glBindVertexArray(ico_object->vertex_array->vao);
            ico_object->vertex_array->draw();
        }

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
