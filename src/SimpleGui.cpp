#include "SimpleGui.hpp"

// ImGui includes.
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

// GLFW include apparently not needed since we are not using any of its methods.
//#include <GLFW/glfw3.h>

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

        ImGui::Text("System input:");
        ImGui::Text("- WASD keys to move the camera.");
        ImGui::Text("- Click and drag to rotate the scene.");
        ImGui::Text("\n");

        ImGui::Text("UI input:");

        ImGui::Checkbox("Perspective projection", &gui_state.is_perspective);

        ImGui::SliderFloat("H value", &gui_state.H, 0.0f, 360.0f);
        ImGui::SliderFloat("S value", &gui_state.S, 0.0f, 1.0f);
        ImGui::SliderFloat("V value", &gui_state.V, 0.0f, 1.0f);

        ImGui::Text("Lighting parameters:");
        ImGui::SliderFloat("Ambient", &gui_state.ambient, 0.0f, 1.0f);
        ImGui::SliderFloat("Diffuse", &gui_state.diffuse, 0.0f, 1.0f);
        ImGui::SliderFloat("Specular", &gui_state.specular, 0.0f, 1.0f);

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
