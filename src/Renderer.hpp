#ifndef RENDERER_HPP
#define RENDERER_HPP

#include "Scene.hpp"
#include "ShaderProgram.hpp"

class Camera;

// Crucial elements:
// - Render target
// - Shader (to Set uniforms)
// - Camera

struct RenderParameter
{
    // Shader.
    int shader = 0;

    // Lighting parameters.
    float ambient = 0.2f;
    float diffuse = 1.0f;
    float specular = 1.0f;
};

class TableSceneRenderer
{
public:

    TableSceneRenderer(int screen_width, int screen_height);

    void renderTableScene(const TableScene& scene, Camera& camera, const RenderParameter& params);
    void renderPhongMaterial();

private:

    ShaderProgram shader_flat_;
    ShaderProgram shader_gouraud_;
    ShaderProgram shader_phong_;
    ShaderProgram shader_light_source_;
    ShaderProgram shader_shadow_;
    ShaderProgram shader_shadow_debug_;

    // Screen resolution.
    int screen_width_;
    int screen_height_;

    // Shadow map variables.
    const int shadow_map_width_;
    const int shadow_map_height_;
    unsigned int depth_map_tex_;
    unsigned int depth_map_fbo_;
};


#endif // RENDERER_HPP
