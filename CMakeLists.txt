cmake_minimum_required(VERSION 3.10)

project(cg-vault)

# Specify the C++ standard.
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Build GLFW.
add_subdirectory(deps/glfw)

# Add demo executable target.
add_executable(
    demo
    # Glad source.
    deps/glad/src/glad.c
    # ImGui source.
    deps/imgui/imgui.cpp
    deps/imgui/imgui_demo.cpp
    deps/imgui/imgui_draw.cpp
    deps/imgui/imgui_tables.cpp
    deps/imgui/imgui_widgets.cpp
    deps/imgui/backends/imgui_impl_glfw.cpp
    deps/imgui/backends/imgui_impl_opengl3.cpp
    # Project source.
    src/Main.cpp
    src/ArcballHandler.cpp
    src/Camera.cpp
    src/Geometry.cpp
    src/Math.cpp
    src/Mesh.cpp
    src/SceneNode.cpp
    src/SimpleGui.cpp
    src/ShaderProgram.cpp
    src/Teapot.cpp
    src/Vertex.cpp
    src/VertexArray.cpp
)

target_include_directories(
    demo
    PRIVATE
        deps/glad/include
        deps/glfw/include
        deps/glm
        deps/imgui
        deps/imgui/backends
        deps/stb
)

target_link_libraries(
    demo
    glfw
)
