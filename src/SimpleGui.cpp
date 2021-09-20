#include "SimpleGui.hpp"

// ImGui includes.
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include <GLFW/glfw3.h>

void setupImGui(GLFWwindow* window)
{
    // Setup Dear ImGui context.
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    // Setup Dear ImGui style.
    ImGui::StyleColorsDark();
    // Setup Platform/Renderer backends.
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    const char* glsl_version = "#version 130";
    ImGui_ImplOpenGL3_Init(glsl_version);
}

void setupGuiFrame(GuiState& gui_state)
{
    assert(gui_state.time_per_frame != 0.0);

    // Start the Dear ImGui frame.
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    // ImGui: use a Begin/End pair to create a named window.
    {
        // Create a window and add elements into it.
        ImGui::Begin("Options");

        ImGui::Checkbox("Perspective projection", &gui_state.is_perspective);

        ImGui::Text("Average time per frame: %.3f ms (%.1f FPS)",
                    gui_state.time_per_frame,
                    1000.0 / gui_state.time_per_frame);

        ImGui::End();
    }
}

void renderGui()
{
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void terminateImGui()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}
