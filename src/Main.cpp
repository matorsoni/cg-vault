#include <cmath>
#include <iostream>

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "ShaderProgram.hpp"
#include "Texture.hpp"

using namespace std;
using glm::mat4;
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

// Create rotation matrix.
static mat4 getRotationZ(float angle)
{
    float s = glm::sin(glm::radians(angle));
    float c = glm::cos(glm::radians(angle));
    // GLM defines matrices as (col0x, col0y, col0z, col0w, ...), always remeber it's transposed.
    return mat4(
        c, s, 0, 0,
       -s, c, 0, 0,
        0, 0, 1, 0,
        0, 0, 0, 1
    );
}

// Create scaling matrix.
static mat4 getScale(float a, float b, float c)
{
    return mat4(
        a, 0, 0, 0,
        0, b, 0, 0,
        0, 0, c, 0,
        0, 0, 0, 1
    );
}

// Create translation matrix.
static mat4 getTranslation(vec3 u)
{
    // Initialize as identity.
    mat4 trans(1.0);
    // Set the fourth column to be the translation vector.
    trans[3] = vec4(u, 1.0);
    return trans;
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

    glfwSwapInterval(1);
    glEnable(GL_DEPTH_TEST);

    // Define triangle vertices.
    float vertices[] = {
        // Position           // Texture
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
    };

    // Create Vertex Array Object.
    unsigned int vao_id;
    glGenVertexArrays(1, &vao_id);
    glBindVertexArray(vao_id);
    // Populate VAO with a Vertex Buffer Object.
    unsigned int vbo_id;
    glGenBuffers(1, &vbo_id);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_id);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    // Specify vertex positions, on layout location 0.
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // Specify vertex texture coords, on layout location 2.
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // Load shader program.
    ShaderProgram shader_program("../src/shader/vertex.glsl", "../src/shader/fragment.glsl");

    // Render in wireframe mode.
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    // Create texture.
    Texture texture0("../assets/wall.jpg");
    Texture texture1("../assets/awesomeface.png");

    double tick = glfwGetTime(), tock;
    while (!glfwWindowShouldClose(window))
    {
        // Get frame buffer size and set OpenGL viewport dimensions.
        // Should normally be the same size as the window.
        float aspect_ratio;
        {
            int width, height;
            glfwGetFramebufferSize(window, &width, &height);
            glViewport(0, 0, width, height);
            aspect_ratio = static_cast<float>(width) / height;
        }

        processInput(window);

        glClearColor(0.0f, 0.2f, 0.2f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Set vertex color via uniform.
        float green = sin(tick) / 2.0f + 0.5f;

        // Create model (local -> world) matrix transformation.
        mat4 model(1.0);
        model = glm::rotate(model, glm::radians(-55.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        model = glm::rotate(model, float(tick) * glm::radians(45.0f), glm::vec3(1.0f, 1.0f, 0.0f));
        // Create camera view matrix transform.
        mat4 view = getTranslation(vec3(0.0f, 0.0f, -3.0f));
        // Create projection matrix.
        mat4 proj = glm::perspective(glm::radians(45.0f), aspect_ratio, 0.1f, 100.0f);
        // Final transformation.
        mat4 transf = proj * view * model;

        // Bind texture.
        texture0.bind(0);
        texture1.bind(1);

        // Update uniform values.
        shader_program.use();
        shader_program.setUniform1i("u_sampler0", 0);
        shader_program.setUniform1i("u_sampler1", 1);
        shader_program.setUniform4f("u_color", 0.0f, green, 0.0f, 1.0f);
        shader_program.setUniformMat4f("u_mat", glm::value_ptr(transf));
        glBindVertexArray(vao_id);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        glfwSwapBuffers(window);
        glfwPollEvents();

        tock = glfwGetTime();
        //cout << "Time per frame: " << 1000.0 * (tock - tick) << endl;
        tick = tock;
    }

    glfwDestroyWindow(window);
    glfwTerminate();
}
