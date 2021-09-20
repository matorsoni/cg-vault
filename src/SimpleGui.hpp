#ifndef SIMPLE_GUI_HPP
#define SIMPLE_GUI_HPP

// Forward declaration.
struct GLFWwindow;

struct GuiState
{
    bool is_perspective = true;
    double time_per_frame = 1.0;
};

void setupImGui(GLFWwindow* window);

void setupGuiFrame(GuiState& gui_state);

void renderGui();

void terminateImGui();

#endif // SIMPLE_GUI_HPP
