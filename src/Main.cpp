#include <cmath>
#include <iostream>
#include <vector>

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>

#include "Math.hpp"
#include "Mesh.hpp"
#include "ShaderProgram.hpp"
#include "Texture.hpp"

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
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
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

    // Set OpenGL constant states.
    glfwSwapInterval(1);
    glEnable(GL_DEPTH_TEST);
    glViewport(0, 0, window_width, window_height);
    const float aspect_ratio = static_cast<float>(window_width) / window_height;

    // Create cube mesh.
    vector<Vertex> cube_vertices;
    createCube(cube_vertices);
    Mesh cube(cube_vertices, {});

    // Create square mesh.
    vector<Vertex> square_vertices;
    vector<unsigned int> square_indices;
    createSquare(square_vertices, square_indices);
    Mesh square(square_vertices, square_indices);

    // Load shader program.
    ShaderProgram shader_program("../src/shader/vertex.glsl", "../src/shader/fragment.glsl");

    // Render in wireframe mode.
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    // Create texture.
    Texture texture0("../assets/wall.jpg");
    Texture texture1("../assets/awesomeface.png");

    // Setup camera with Y as the up direction.
    const vec3 up{0.0f, 1.0f, 0.0f};
    vec3 camera_pos{3.0f, 4.0f, 3.0f};
    vec3 target{0.0f};

    /*** Setup the table scene. ***/
    // Table variables.
    const float table_width = 2.0f;
    const float table_depth = 0.75f;
    const float table_height = 1.0f;
    const float leg_width = 0.1f;
    const float top_girth = 0.1f;
    const float top_scale_factor = 1.1f;
    const vec3 leg_scale = vec3(leg_width, table_height, leg_width);
    vec3 leg_position[] = {
        vec3(table_depth/2, table_height/2, table_width/2),
        vec3(table_depth/2, table_height/2, -table_width/2),
        vec3(-table_depth/2, table_height/2, -table_width/2),
        vec3(-table_depth/2, table_height/2, table_width/2)
    };
    const vec3 top_scale = vec3(top_scale_factor * table_depth, top_girth, top_scale_factor * table_width);
    const vec3 top_position = vec3(0.0f, table_height + top_girth/2, 0.0f);

    // Main loop.
    double tick = glfwGetTime(), tock;
    while (!glfwWindowShouldClose(window))
    {
        processInput(window);

        glClearColor(0.0f, 0.2f, 0.2f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Set vertex color via uniform.
        float green = sin(tick) / 2.0f + 0.5f;

        // Create camera view matrix transform.
        mat4 view = getView(camera_pos, up, target);
        // Create projection matrix.
        mat4 proj = glm::perspective(glm::radians(45.0f), aspect_ratio, 0.1f, 100.0f);
        // Final transformation.
        mat4 transf = proj * view;

        // Bind texture.
        texture0.bind(0);

        // Draw table.
        shader_program.use();
        shader_program.setUniform1i("u_sampler0", 0);
        shader_program.setUniform1i("u_sampler1", 1);
        shader_program.setUniform4f("u_color", 0.0f, green, 0.0f, 1.0f);
        shader_program.setUniformMat4f("u_mat", glm::value_ptr(transf));
        mat4 model(1.0f);
        glBindVertexArray(cube.vao);
        // Draw table top.
        model = getScale(top_scale);
        model = getTranslation(top_position) * model;
        shader_program.setUniformMat4f("u_model", glm::value_ptr(model));
        cube.draw();
        // Draw legs.
        for (int i = 0; i < 4; ++i) {
            // Create model (local -> world) matrix transformation.
            model = getScale(leg_scale);
            model = getTranslation(leg_position[i]) * model;
            shader_program.setUniformMat4f("u_model", glm::value_ptr(model));

            cube.draw();
        }

        model = getScale(vec3(5.0f, 0.0f, 5.0f));
        texture0.unbind();
        texture1.bind(1);
        shader_program.setUniformMat4f("u_model", glm::value_ptr(model));
        glBindVertexArray(square.vao);
        square.draw();
        texture1.unbind();

        glfwSwapBuffers(window);
        glfwPollEvents();

        tock = glfwGetTime();
        //cout << "Time per frame: " << 1000.0 * (tock - tick) << endl;
        tick = tock;
    }

    glfwDestroyWindow(window);
    glfwTerminate();
}
