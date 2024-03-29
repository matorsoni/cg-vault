#ifndef SIMPLE_GUI_HPP
#define SIMPLE_GUI_HPP

// Forward declaration.
struct GLFWwindow;

struct GuiState
{
    int is_perspective = true;
    double time_per_frame = 1.0;
    float H = 0.0f;
    float S = 1.0f;
    float V = 1.0f;

    // Lighting parameters.
    float ambient = 0.2f;
    float diffuse = 1.0f;
    float specular = 1.0f;

    // Teapot texture.
    int teapot_tex = 3;
};

void setupImGui(GLFWwindow* window);

void setupGuiFrame(GuiState& gui_state);

void renderGui();

void terminateImGui();

#endif // SIMPLE_GUI_HPP
