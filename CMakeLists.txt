cmake_minimum_required(VERSION 3.10)

project(cg-vault)

# Build GLFW.
add_subdirectory(deps/glfw)

# Add demo executable target.
add_executable(
    demo
    deps/glad/src/glad.c
    src/Main.cpp
    src/Geometry.cpp
    src/Math.cpp
    src/Mesh.cpp
    src/ShaderProgram.cpp
    src/Teapot.cpp
    src/Texture.cpp
)

target_include_directories(
    demo
    PRIVATE
        deps/glad/include
        deps/glfw/include
        deps/glm
        deps/stb
)

target_link_libraries(
    demo
    glfw
)
