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

    // Create debug quad.
    Mesh quad_mesh = createQuad();
    VertexArray quad(quad_mesh);

    //ShaderProgram shader_normal("../src/shader/ClippingPlane.vert",
    //                            "../src/shader/VertexColor.frag");
    //ShaderProgram shader_single_color("../src/shader/ClippingPlaneSingleColor.vert",
    //                                  "../src/shader/VertexColor.frag");
    ShaderProgram shader_flat("../src/shader/Flat.vert",
                              "../src/shader/FlatVertexColor.frag");
    ShaderProgram shader_gouraud("../src/shader/GouraudSingleColor.vert",
                                 "../src/shader/VertexColor.frag");
    ShaderProgram shader_phong("../src/shader/Phong.vert",
                               "../src/shader/Phong.frag");
    ShaderProgram shader_light_source("../src/shader/LightSource.vert",
                                      "../src/shader/VertexColor.frag");
    ShaderProgram shader_shadow("../src/shader/Shadow.vert",
                                "../src/shader/Shadow.frag");
    ShaderProgram shader_shadow_debug("../src/shader/Debug.vert",
                                      "../src/shader/Debug.frag");

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

    // Light source.
    SceneNode* point_light = scene.makeSubnode();
    point_light->pos = vec3{0.0f, 2.0f, 0.0f};
    point_light->scale = vec3(0.1f);
    point_light->vertex_array = &cube;

    // Materials.
    PhongMaterial ico_material;
    PhongMaterial torus_material;
    PhongMaterial table_material;
        table_material.ka = vec3(128.f, 83.f, 0.f) / 255.f;
        table_material.kd = vec3(181.f, 88.f, 0.f) / 255.f;
    PhongMaterial teapot_material;
        teapot_material.ka = vec3(0.f, 0.f, 1.f);
        teapot_material.kd = vec3(0.f, 213.f, 255.f) / 255.f;
        teapot_material.shiny = 100.f;

    // Setup camera.
    Camera camera(window_width, window_height);
    camera.position() = vec3(2.7f, 2.7f, 2.7f);
    camera.lookAt(vec3(0.0f, table_top_y, 0.0f));

    // Setup Arcball handler.
    ArcballHandler arcball(window_width, window_height);

    // Setup ImGui and GUI state.
    setupImGui(window);
    GuiState gui_state;

    // Shadow stuff.
    const unsigned int SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;
    unsigned int depthMap;
    glGenTextures(1, &depthMap);
    glBindTexture(GL_TEXTURE_2D, depthMap);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32,
                 SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    unsigned int depthMapFBO;
    glGenFramebuffers(1, &depthMapFBO);
    glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
    cout << glCheckFramebufferStatus(GL_FRAMEBUFFER) << " " << GL_FRAMEBUFFER_COMPLETE << endl;
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

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

        // --- Shadow pass --- //
        glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
        glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
        glClear(GL_DEPTH_BUFFER_BIT);

        // World light position.
        point_light->pos = vec3{cosf(tock), 6.2f, sinf(tock)};
        vec3 light_position = vec3(point_light->worldTransformation()[3]);

        Camera light_source_camera(SHADOW_WIDTH, SHADOW_HEIGHT);
        light_source_camera.position() = light_position;
        light_source_camera.lookAt(vec3(0.0f, 0.0f, 0.0f));
        shader_shadow.use();
        shader_shadow.setUniformMat4f("u_light_view",
                                      glm::value_ptr(light_source_camera.view()));
        shader_shadow.setUniformMat4f("u_light_projection",
                                      glm::value_ptr(light_source_camera.projection()));
        // Draw table for shadow pass.
        for (int i = 0; i < table_object->subnodes.size(); ++i) {
            auto* node = table_object->subnodes[i].get();
            auto model = node->worldTransformation();
            shader_shadow.setUniformMat4f("u_model", glm::value_ptr(model));
            glBindVertexArray(node->vertex_array->vao);
            node->vertex_array->draw();
        }

        // Draw torus for shadow pass.
        {
            auto model = torus_object->worldTransformation();
            shader_shadow.setUniformMat4f("u_model", glm::value_ptr(model));
            glBindVertexArray(torus_object->vertex_array->vao);
            torus_object->vertex_array->draw();
        }

        // Draw teapot for shadow pass.
        {
            auto model = teapot_object->worldTransformation();
            shader_shadow.setUniformMat4f("u_model", glm::value_ptr(model));
            glBindVertexArray(teapot_object->vertex_array->vao);
            teapot_object->vertex_array->draw();
        }

        // Draw icosahedron for shadow pass.
        {
            auto model = ico_object->worldTransformation();
            shader_shadow.setUniformMat4f("u_model", glm::value_ptr(model));
            glBindVertexArray(ico_object->vertex_array->vao);
            ico_object->vertex_array->draw();
        }

        // Render pass.
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glViewport(0, 0, window_width, window_height);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, depthMap);

        //shader_shadow_debug.use();
        //shader_shadow_debug.setUniform1i("shadow_map", 0);
        //glBindVertexArray(quad.vao);
        //quad.draw();

        // Determine shader to be used.
        ShaderProgram& shader = //gui_state.shader == 0 ? shader_flat :
                                //gui_state.shader == 1 ? shader_gouraud :
                                //gui_state.shader == 2 ? shader_phong   :
                                shader_phong;

        shader.use();
        shader.setUniformMat4f("u_view", glm::value_ptr(camera.view()));
        shader.setUniformMat4f("u_projection", glm::value_ptr(camera.projection()));
        shader.setUniformMat4f("u_light_view",
                               glm::value_ptr(light_source_camera.view()));
        shader.setUniformMat4f("u_light_projection",
                               glm::value_ptr(light_source_camera.projection()));
        shader.setUniform1i("shadow_map", 0);

        shader.setUniform3f("u_light_position",
                            light_position.x,
                            light_position.y,
                            light_position.z);

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

        // Draw light source.
        {
            shader_light_source.use();
            shader_light_source.setUniformMat4f("u_view", glm::value_ptr(camera.view()));
            shader_light_source.setUniformMat4f("u_projection", glm::value_ptr(camera.projection()));
            auto model = point_light->worldTransformation();
            shader_light_source.setUniformMat4f("u_model", glm::value_ptr(model));
            glBindVertexArray(point_light->vertex_array->vao);
            point_light->vertex_array->draw();
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
