#include <cmath>
#include <iostream>
#include <vector>

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>

#include "Math.hpp"
#include "Geometry.hpp"
#include "SceneNode.hpp"
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
static void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GLFW_TRUE);
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

    cout << "OpenGL version " << glGetString(GL_VERSION) << endl;

    // Set GLFW swap interval.
    glfwSwapInterval(1);
    // Set OpenGL constant states.
    glEnable(GL_DEPTH_TEST);
    //glCullFace(GL_BACK);
    //glFrontFace(GL_CCW);
    //glEnable(GL_CULL_FACE);

    glViewport(0, 0, window_width, window_height);
    const float aspect_ratio = static_cast<float>(window_width) / window_height;

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

    // Icosahedron position.
    vec3 ico_position{0.0f, table_top_y + 0.8f, -0.5f};

    // Setup camera with Y as the up direction.
    const vec3 up{0.0f, 1.0f, 0.0f};
    vec3 camera_pos{2.7f, 2.7f, 2.7f};
    vec3 target{0.0f, table_top_y, 0.0f};

    // Main loop.
    double tick = glfwGetTime(), tock;
    while (!glfwWindowShouldClose(window))
    {
        processInput(window);

        glClearColor(0.0f, 0.2f, 0.2f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Create camera view matrix transform.
        mat4 view = getView(camera_pos, up, target);
        // Create projection matrix.
        mat4 proj = glm::perspective(glm::radians(45.0f), aspect_ratio, 0.1f, 100.0f);
        // Final transformation.
        mat4 transf = proj * view;

        // Draw table.
        shader_normal.use();
        shader_normal.setUniformMat4f("u_view_projection", glm::value_ptr(transf));
        for (int i = 0; i < table_object->subnodes.size(); ++i) {
            auto* node = table_object->subnodes[i].get();
            auto model = node->worldTransformation();
            shader_normal.setUniformMat4f("u_model", glm::value_ptr(model));
            glBindVertexArray(node->vertex_array->vao);
            node->vertex_array->draw();
        }

        // Draw icosahedron.
        mat4 model(1.0f);
        model = getScale(vec3(0.35f));
        model = getRotationY(50 * tick) * model;
        model = getTranslation(ico_position) * model;
        shader_normal.use();
        shader_normal.setUniformMat4f("u_model", glm::value_ptr(model));
        shader_normal.setUniformMat4f("u_view_projection", glm::value_ptr(transf));
        glBindVertexArray(icosahedron.vao);
        // Draw wireframe.
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        icosahedron.draw();
        // Turn back to fill triangles.
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

        // Draw teapot.
        glBindVertexArray(teapot.vao);
        model = mat4(1.0f);
        model = getScale({0.2f, 0.2f, 0.2f}) * model;
        model = getRotationX(-90) * model;
        model = getRotationY(50 * tick) * model;
        model = getTranslation(vec3(0.0f, table_top_y + 0.01f, 0.65f)) * model;
        shader_normal.use();
        shader_normal.setUniformMat4f("u_model", glm::value_ptr(model));
        shader_normal.setUniformMat4f("u_view_projection", glm::value_ptr(transf));
        //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        teapot.draw();
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

        glfwSwapBuffers(window);
        glfwPollEvents();

        tock = glfwGetTime();
        //cout << "Time per frame: " << 1000.0 * (tock - tick) << endl;
        tick = tock;
    }

    glfwDestroyWindow(window);
    glfwTerminate();
}
