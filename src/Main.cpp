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

    // Define cube vertices.
    vector<Vertex> cube_vertices = {
        // Position                 // Normal   // Texture
        {vec3(-0.5f, -0.5f, -0.5f), vec3(0.0f), vec2(0.0f, 0.0f)},
        {vec3( 0.5f, -0.5f, -0.5f), vec3(0.0f), vec2(1.0f, 0.0f)},
        {vec3( 0.5f,  0.5f, -0.5f), vec3(0.0f), vec2(1.0f, 1.0f)},
        {vec3( 0.5f,  0.5f, -0.5f), vec3(0.0f), vec2(1.0f, 1.0f)},
        {vec3(-0.5f,  0.5f, -0.5f), vec3(0.0f), vec2(0.0f, 1.0f)},
        {vec3(-0.5f, -0.5f, -0.5f), vec3(0.0f), vec2(0.0f, 0.0f)},

        {vec3(-0.5f, -0.5f,  0.5f), vec3(0.0f), vec2(0.0f, 0.0f)},
        {vec3( 0.5f, -0.5f,  0.5f), vec3(0.0f), vec2(1.0f, 0.0f)},
        {vec3( 0.5f,  0.5f,  0.5f), vec3(0.0f), vec2(1.0f, 1.0f)},
        {vec3( 0.5f,  0.5f,  0.5f), vec3(0.0f), vec2(1.0f, 1.0f)},
        {vec3(-0.5f,  0.5f,  0.5f), vec3(0.0f), vec2(0.0f, 1.0f)},
        {vec3(-0.5f, -0.5f,  0.5f), vec3(0.0f), vec2(0.0f, 0.0f)},

        {vec3(-0.5f,  0.5f,  0.5f), vec3(0.0f), vec2(1.0f, 0.0f)},
        {vec3(-0.5f,  0.5f, -0.5f), vec3(0.0f), vec2(1.0f, 1.0f)},
        {vec3(-0.5f, -0.5f, -0.5f), vec3(0.0f), vec2(0.0f, 1.0f)},
        {vec3(-0.5f, -0.5f, -0.5f), vec3(0.0f), vec2(0.0f, 1.0f)},
        {vec3(-0.5f, -0.5f,  0.5f), vec3(0.0f), vec2(0.0f, 0.0f)},
        {vec3(-0.5f,  0.5f,  0.5f), vec3(0.0f), vec2(1.0f, 0.0f)},

        {vec3( 0.5f,  0.5f,  0.5f), vec3(0.0f), vec2(1.0f, 0.0f)},
        {vec3( 0.5f,  0.5f, -0.5f), vec3(0.0f), vec2(1.0f, 1.0f)},
        {vec3( 0.5f, -0.5f, -0.5f), vec3(0.0f), vec2(0.0f, 1.0f)},
        {vec3( 0.5f, -0.5f, -0.5f), vec3(0.0f), vec2(0.0f, 1.0f)},
        {vec3( 0.5f, -0.5f,  0.5f), vec3(0.0f), vec2(0.0f, 0.0f)},
        {vec3( 0.5f,  0.5f,  0.5f), vec3(0.0f), vec2(1.0f, 0.0f)},

        {vec3(-0.5f, -0.5f, -0.5f), vec3(0.0f), vec2(0.0f, 1.0f)},
        {vec3( 0.5f, -0.5f, -0.5f), vec3(0.0f), vec2(1.0f, 1.0f)},
        {vec3( 0.5f, -0.5f,  0.5f), vec3(0.0f), vec2(1.0f, 0.0f)},
        {vec3( 0.5f, -0.5f,  0.5f), vec3(0.0f), vec2(1.0f, 0.0f)},
        {vec3(-0.5f, -0.5f,  0.5f), vec3(0.0f), vec2(0.0f, 0.0f)},
        {vec3(-0.5f, -0.5f, -0.5f), vec3(0.0f), vec2(0.0f, 1.0f)},

        {vec3(-0.5f,  0.5f, -0.5f), vec3(0.0f), vec2(0.0f, 1.0f)},
        {vec3( 0.5f,  0.5f, -0.5f), vec3(0.0f), vec2(1.0f, 1.0f)},
        {vec3( 0.5f,  0.5f,  0.5f), vec3(0.0f), vec2(1.0f, 0.0f)},
        {vec3( 0.5f,  0.5f,  0.5f), vec3(0.0f), vec2(1.0f, 0.0f)},
        {vec3(-0.5f,  0.5f,  0.5f), vec3(0.0f), vec2(0.0f, 0.0f)},
        {vec3(-0.5f,  0.5f, -0.5f), vec3(0.0f), vec2(0.0f, 1.0f)}
    };

    // Create Mesh.
    Mesh cube(cube_vertices, {});

    // Load shader program.
    ShaderProgram shader_program("../src/shader/vertex.glsl", "../src/shader/fragment.glsl");

    // Render in wireframe mode.
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    // Create texture.
    Texture texture0("../assets/wall.jpg");
    Texture texture1("../assets/awesomeface.png");

    // Render more cubes.
    vec3 cube_positions[10] = {
        vec3(0.0f, 0.0f, -10.0f),
        vec3(1.0f, 1.0f, -1.0f),
        vec3(2.0f, 0.0f, -2.0f),
        vec3(-2.0f, -2.0f, -4.0f),
        vec3(1.0f, -1.0f, -4.0f),
        vec3(-4.0f, -3.0f, -8.0f),
        vec3(-3.0f, 0.0f, -4.0f),
        vec3(3.0f, -3.0f, -10.0f),
        vec3(2.0f, 0.0f, -4.0f),
        vec3(0.0f, 0.0f, -4.0f)
    };

    // Y as the up direction.
    const vec3 up{0.0f, 1.0f, 0.0f};
    vec3 camera_pos{0.0f, 1.0f, 1.0f};
    vec3 target{0.0f};

    double tick = glfwGetTime(), tock;
    while (!glfwWindowShouldClose(window))
    {
        processInput(window);

        glClearColor(0.0f, 0.2f, 0.2f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Set vertex color via uniform.
        float green = sin(tick) / 2.0f + 0.5f;

        // Create camera view matrix transform.
        camera_pos.z += 0.05;
        mat4 view = getView(camera_pos, up, target);
        // Create projection matrix.
        mat4 proj = glm::perspective(glm::radians(45.0f), aspect_ratio, 0.1f, 100.0f);
        // Final transformation.
        mat4 transf = proj * view;

        // Bind texture.
        texture0.bind(0);
        texture1.bind(1);

        // Update uniform values.
        shader_program.use();
        shader_program.setUniform1i("u_sampler0", 0);
        shader_program.setUniform1i("u_sampler1", 1);
        shader_program.setUniform4f("u_color", 0.0f, green, 0.0f, 1.0f);
        shader_program.setUniformMat4f("u_mat", glm::value_ptr(transf));
        glBindVertexArray(cube.vao);
        for (int i = 0; i < 10; ++i) {
            // Create model (local -> world) matrix transformation.
            mat4 model = getTranslation(cube_positions[i]);
            model = glm::rotate(model, glm::radians(18.0f * i), glm::vec3(1.0f, 0.0f, 0.0f));
            model = glm::rotate(model, float(tick) * glm::radians(45.0f), glm::vec3(1.0f, 1.0f, 0.0f));
            shader_program.setUniformMat4f("u_model", glm::value_ptr(model));

            cube.draw();
        }

        glfwSwapBuffers(window);
        glfwPollEvents();

        tock = glfwGetTime();
        //cout << "Time per frame: " << 1000.0 * (tock - tick) << endl;
        tick = tock;
    }

    glfwDestroyWindow(window);
    glfwTerminate();
}
