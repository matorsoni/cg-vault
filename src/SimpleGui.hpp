#ifndef SIMPLE_GUI_HPP
#define SIMPLE_GUI_HPP

// Forward declaration.
struct GLFWwindow;

struct GuiState
{
    bool is_perspective = true;
    double time_per_frame = 1.0;
    float H = 0.0f;
    float S = 1.0f;
    float V = 1.0f;
    // Lighting parameters.
    float K_A = 0.2f;
    float K_D = 1.0f;
};

void setupImGui(GLFWwindow* window);

void setupGuiFrame(GuiState& gui_state);

void renderGui();

void terminateImGui();

#endif // SIMPLE_GUI_HPP
