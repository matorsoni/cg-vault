#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glad/glad.h>

#include <iostream>

// GLFW error calback.
static void error_callback(int error, const char* description)
{
    fprintf(stderr, "Error: %s\n", description);
}

// GLFW key event callback.
static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GLFW_TRUE);
}

// Main function.
int main()
{
    glfwSetErrorCallback(error_callback);

    if (!glfwInit()) {
        std::cout << "Failed to initialize GLFW." << std::endl;
        return -1;
    }

    const int window_width = 640;
    const int window_height = 480;
    // Set minimum OpenGL version expected by the context.
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    GLFWwindow* window = glfwCreateWindow(window_width, window_height, "Demo", NULL, NULL);
    if (!window) {
        std::cout << "Failed to create window." << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    if (!gladLoadGLLoader( (GLADloadproc)glfwGetProcAddress) ) {
        std::cout << "Failed to initialize OpenGL context." << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwSwapInterval(1);
    glfwSetKeyCallback(window, key_callback);

    // Define triangle vertices.
    float vertices[] = {
        -0.5f, -0.5f, 0.0f,
         0.5f, -0.5f, 0.0f,
         0.0f,  0.5f, 0.0f
    };

    // Create Vertex Array Object.
    unsigned int vao_id;
    glGenVertexArrays(1, &vao_id);
    glBindVertexArray(vao_id);
    // Populate VAO with a Vertex Buffer Object.
    unsigned int vbo_id;
    glGenBuffers(1, &vbo_id);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_id);
    glBufferData(GL_ARRAY_BUFFER, 9 * sizeof(float), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Simple vertex shader.
    const char *vert_shader_src =
            "#version 330 core\n"
            "layout (location = 0) in vec3 aPos;\n"
            "void main()\n"
            "{\n"
            "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
            "}\0";
    unsigned int vert_shader_id = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vert_shader_id, 1, &vert_shader_src, NULL);
    glCompileShader(vert_shader_id);

    // Simple fragment shader.
    const char *frag_shader_src =
            "#version 330 core\n"
            "out vec4 FragColor;\n"
            "void main()\n"
            "{\n"
            "    FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
            "}\0";
    unsigned int frag_shader_id = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(frag_shader_id, 1, &frag_shader_src, NULL);
    glCompileShader(frag_shader_id);

    // Check if shaders compiled correctly.
    {
        int success;
        char info_log[512];

        glGetShaderiv(vert_shader_id, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(vert_shader_id, 512, NULL, info_log);
            std::cout << "Vertex shader compilation failed: \n" << info_log << std::endl;
        }

        glGetShaderiv(frag_shader_id, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(frag_shader_id, 512, NULL, info_log);
            std::cout << "Fragment shader compilation failed: \n" << info_log << std::endl;
        }
    }

    // Linking shader program.
    unsigned int shader_program_id = glCreateProgram();
    glAttachShader(shader_program_id, vert_shader_id);
    glAttachShader(shader_program_id, frag_shader_id);
    glLinkProgram(shader_program_id);

    // Check if linking went well.
    {
        int success;
        char info_log[512];
        glGetProgramiv(shader_program_id, GL_LINK_STATUS, &success);
        if (!success) {
            glGetProgramInfoLog(shader_program_id, 512, NULL, info_log);
            std::cout << "Shader program linking failed: \n" << info_log << std::endl;

        }
    }

    // Delete intermediate shader objects.
    glDeleteShader(vert_shader_id);
    glDeleteShader(frag_shader_id);


    double tick = glfwGetTime(), tock;
    while (!glfwWindowShouldClose(window))
    {
        // Get frame buffer size and set OpenGL viewport dimensions.
        // Should normally be the same size as the window.
        {
            int width, height;
            glfwGetFramebufferSize(window, &width, &height);
            glViewport(0, 0, width, height);
        }

        glClearColor(0.0f, 0.0f, 0.2f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // Draw calls.
        glUseProgram(shader_program_id);
        glBindVertexArray(vao_id);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        glfwSwapBuffers(window);
        glfwPollEvents();

        tock = glfwGetTime();
        //std::cout << "Time per frame: " << 1000.0 * (tock - tick) << std::endl;
        tick = tock;
    }

    glfwDestroyWindow(window);
    glfwTerminate();
}
