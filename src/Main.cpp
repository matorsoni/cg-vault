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
    ShaderProgram shader_texture("../src/shader/vertex.glsl", "../src/shader/fragment.glsl");
    ShaderProgram shader_normal("../src/shader/VertexColorFromNormal.vert", "../src/shader/VertexColor.frag");

    // Render in wireframe mode.
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    // Create texture.
    Texture texture0("../assets/wood3.jpeg");
    Texture texture1("../assets/wood1.jpeg");
    Texture texture2("../assets/black-brick2.jpg");
    Texture texture3("../assets/chess.jpeg");

    /*** Setup the table scene. ***/
    // Table variables.
    const float table_width = 2.0f;
    const float table_depth = 1.0f;
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
    const vec3 top_scale = vec3(top_scale_factor * table_depth,
                                top_girth,
                                top_scale_factor * table_width);
    const vec3 top_position = vec3(0.0f, table_height + top_girth/2, 0.0f);
    const float table_top_y = table_height + top_girth;

    // Icosahedron.
    vector<Vertex> ico_vertices;
    vector<unsigned int> ico_indices;
    createIcosahedron(ico_vertices, ico_indices);
    Mesh icosahedron(ico_vertices, ico_indices);
    vec3 ico_position{0.0f, table_top_y + 0.8f, -0.5f};

    // Floor and walls.
    const float floor_width = 5.0f;
    const float floor_depth = 5.0f;
    const vec3 wall_1_pos = vec3(-floor_width/2, floor_depth/2, 0.0f);
    const vec3 wall_2_pos = vec3(0.0f, floor_depth/2, -floor_width/2);
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

        // Set vertex color via uniform.
        float green = sin(tick) / 2.0f + 0.5f;

        // Create camera view matrix transform.
        mat4 view = getView(camera_pos, up, target);
        // Create projection matrix.
        mat4 proj = glm::perspective(glm::radians(45.0f), aspect_ratio, 0.1f, 100.0f);
        // Final transformation.
        mat4 transf = proj * view;

        // Draw table.
        shader_texture.use();
        shader_texture.setUniform1i("u_sampler", 0);
        shader_texture.setUniformMat4f("u_mat", glm::value_ptr(transf));
        texture0.bind(0);
        // Start with the table top.
        mat4 model(1.0f);
        model = getScale(top_scale);
        model = getTranslation(top_position) * model;
        shader_texture.setUniformMat4f("u_model", glm::value_ptr(model));
        glBindVertexArray(cube.vao);
        cube.draw();
        // Draw legs.
        for (int i = 0; i < 4; ++i) {
            // Create model (local -> world) matrix transformation.
            model = getScale(leg_scale);
            model = getTranslation(leg_position[i]) * model;
            shader_texture.setUniformMat4f("u_model", glm::value_ptr(model));

            cube.draw();
        }
        texture0.unbind();

        // Draw icosahedron.
        model = getScale(vec3(0.35f));
        model = getRotationY(50 * tick) * model;
        model = getTranslation(ico_position) * model;
        shader_normal.use();
        shader_normal.setUniformMat4f("u_model", glm::value_ptr(model));
        shader_normal.setUniformMat4f("u_mat", glm::value_ptr(transf));
        glBindVertexArray(icosahedron.vao);
        // Draw wireframe.
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        icosahedron.draw();
        // Turn back to fill triangles.
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

        // Draw floor.
        texture1.bind(0);
        model = getScale(vec3(floor_depth, 0.0f, floor_width));
        shader_texture.use();
        shader_texture.setUniform1i("u_sampler", 0);
        shader_texture.setUniformMat4f("u_model", glm::value_ptr(model));
        shader_texture.setUniformMat4f("u_mat", glm::value_ptr(transf));
        glBindVertexArray(square.vao);
        square.draw();
        texture1.unbind();
        // Draw walls.
        texture2.bind(0);
        model = getScale(vec3(floor_depth, 0.0f, floor_width));
        model = getRotationX(90.0f) * getRotationZ(-90.0f) * model;
        model = getTranslation(wall_1_pos) * model;
        shader_texture.setUniformMat4f("u_model", glm::value_ptr(model));
        shader_texture.setUniformMat4f("u_mat", glm::value_ptr(transf));
        square.draw();
        model = getScale(vec3(floor_depth, 0.0f, floor_width));
        model = getRotationX(-90.0f) * model;
        model = getTranslation(wall_2_pos) * model;
        shader_texture.setUniformMat4f("u_model", glm::value_ptr(model));
        shader_texture.setUniformMat4f("u_mat", glm::value_ptr(transf));
        square.draw();
        texture2.unbind();
        // Draw chessboard.
        texture3.bind(0);
        model = getScale(vec3(0.8f));
        model = getRotationY(60.0f) * model;
        model = getTranslation(vec3(0.0f, table_top_y + 0.01f, -0.25f));
        shader_texture.setUniformMat4f("u_model", glm::value_ptr(model));
        square.draw();

        glfwSwapBuffers(window);
        glfwPollEvents();

        tock = glfwGetTime();
        //cout << "Time per frame: " << 1000.0 * (tock - tick) << endl;
        tick = tock;
    }

    glfwDestroyWindow(window);
    glfwTerminate();
}
