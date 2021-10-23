cmake_minimum_required(VERSION 3.10)

project(vault-lib)

# Add demo executable target.
add_executable(
    demo
    # Glad source.
    ${VAULT_DEPS_DIR}/glad/src/glad.c
    # ImGui source.
    ${VAULT_DEPS_DIR}/imgui/imgui.cpp
    ${VAULT_DEPS_DIR}/imgui/imgui_demo.cpp
    ${VAULT_DEPS_DIR}/imgui/imgui_draw.cpp
    ${VAULT_DEPS_DIR}/imgui/imgui_tables.cpp
    ${VAULT_DEPS_DIR}/imgui/imgui_widgets.cpp
    ${VAULT_DEPS_DIR}/imgui/backends/imgui_impl_glfw.cpp
    ${VAULT_DEPS_DIR}/imgui/backends/imgui_impl_opengl3.cpp
    # Project source.
    Main.cpp
    ArcballHandler.cpp
    Camera.cpp
    Geometry.cpp
    Math.cpp
    Mesh.cpp
    SceneNode.cpp
    SimpleGui.cpp
    ShaderProgram.cpp
    Teapot.cpp
    Vertex.cpp
    VertexArray.cpp
)

target_include_directories(
    demo
    PRIVATE
        ${VAULT_DEPS_DIR}/glad/include
        ${VAULT_DEPS_DIR}/glfw/include
        ${VAULT_DEPS_DIR}/glm
        ${VAULT_DEPS_DIR}/imgui
        ${VAULT_DEPS_DIR}/imgui/backends
        ${VAULT_DEPS_DIR}/stb
)

# Build GLFW.
add_subdirectory(${VAULT_DEPS_DIR}/glfw build)

target_link_libraries(
    demo
    glfw
)
