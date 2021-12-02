#include "Renderer.hpp"

#include <iostream>

#include <glad/glad.h>
#include <glm/vec3.hpp>

#include "Camera.hpp"
#include "Scene.hpp"

using namespace std;
using glm::vec3;

TableSceneRenderer::TableSceneRenderer(int screen_width, int screen_height):
    shader_phong_("../src/shader/Phong.vert",
                  "../src/shader/Phong.frag"),
    shader_light_source_("../src/shader/LightSource.vert",
                         "../src/shader/VertexColor.frag"),
    shader_shadow_("../src/shader/Shadow.vert",
                   "../src/shader/Shadow.frag"),
    shader_shadow_debug_("../src/shader/Debug.vert",
                         "../src/shader/Debug.frag"),
    screen_width_(screen_width),
    screen_height_(screen_height),
    shadow_map_width_(1024),
    shadow_map_height_(1024),
    depth_map_tex_(0),
    depth_map_fbo_(0)
{
    // Setup shadow map texture.
    glGenTextures(1, &depth_map_tex_);
    glBindTexture(GL_TEXTURE_2D, depth_map_tex_);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32,
                 shadow_map_width_, shadow_map_height_, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    // Setup shadow map Framebuffer Object.
    glGenFramebuffers(1, &depth_map_fbo_);
    glBindFramebuffer(GL_FRAMEBUFFER, depth_map_fbo_);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depth_map_tex_, 0);
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        cout << "Framebuffer not complete..." << endl;
    }

    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void TableSceneRenderer::renderTableScene(const TableScene& scene,
                                          const Camera& camera,
                                          const RenderParameter& params)
{
    // --- Shadow pass --- //
    glViewport(0, 0, shadow_map_width_, shadow_map_height_);
    glBindFramebuffer(GL_FRAMEBUFFER, depth_map_fbo_);
    glClear(GL_DEPTH_BUFFER_BIT);

    // Define light source camera.
    Camera light_source_camera(static_cast<float>(shadow_map_width_) / shadow_map_height_);
    vec3 light_position = vec3(scene.point_light_node->worldTransformation()[3]);
    light_source_camera.setPosition(light_position);
    light_source_camera.lookAt(vec3(0.0f, 0.0f, 0.0f));
    light_source_camera.updateView();

    shader_shadow_.use();
    shader_shadow_.setUniformMat4f("u_light_view", light_source_camera.view());
    shader_shadow_.setUniformMat4f("u_light_projection", light_source_camera.projection());

    // Draw table for shadow pass.
    for (int i = 0; i < scene.table_node->subnodes.size(); ++i) {
        auto* node = scene.table_node->subnodes[i].get();
        auto model = node->worldTransformation();
        shader_shadow_.setUniformMat4f("u_model", model);
        node->mesh->draw();
    }

    // Draw torus for shadow pass.
    {
        auto model = scene.torus_node->worldTransformation();
        shader_shadow_.setUniformMat4f("u_model", model);
        scene.torus_node->mesh->draw();
    }

    // Draw teapot for shadow pass.
    {
        auto model = scene.teapot_node->worldTransformation();
        shader_shadow_.setUniformMat4f("u_model", model);
        scene.teapot_node->mesh->draw();
    }

    // Draw icosahedron for shadow pass.
    {
        auto model = scene.ico_node->worldTransformation();
        shader_shadow_.setUniformMat4f("u_model", model);
        scene.ico_node->mesh->draw();
    }

    // Draw floor for shadow pass.
    {
        auto model = scene.floor_node->worldTransformation();
        shader_shadow_.setUniformMat4f("u_model", model);
        scene.floor_node->mesh->draw();
    }

    // Render pass.
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glViewport(0, 0, screen_width_, screen_height_);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, depth_map_tex_);

    //shader_shadow_debug.use();
    //shader_shadow_debug.setUniform1i("shadow_map", 0);
    //glBindVertexArray(quad.vao);
    //quad.draw();

    // Determine shader to be used.
    ShaderProgram& shader = shader_phong_;

    shader.use();
    shader.setUniformMat4f("u_view", camera.view());
    shader.setUniformMat4f("u_projection", camera.projection());
    shader.setUniformMat4f("u_light_view", light_source_camera.view());
    shader.setUniformMat4f("u_light_projection", light_source_camera.projection());
    shader.setUniform1i("shadow_map", 0);
    shader.setUniformVec3f("u_light_position", light_position);

    // Update lighting parameters from GUI.
    shader.setUniform1f("u_ambient_coef", params.ambient);
    shader.setUniform1f("u_diffuse_coef", params.diffuse);
    shader.setUniform1f("u_specular_coef", params.specular);

    // Set texture sampler slot.
    const int sampler_slot = 1;
    shader.setUniform1i("object_texture", sampler_slot);

    // Draw table.
    for (int i = 0; i < scene.table_node->subnodes.size(); ++i) {
        auto* node = scene.table_node->subnodes[i].get();
        auto model = node->worldTransformation();
        shader.setUniformMat4f("u_model", model);
        shader.setUniformVec3f("u_ka", scene.table_material.ka);
        shader.setUniformVec3f("u_kd", scene.table_material.kd);
        shader.setUniformVec3f("u_ks", scene.table_material.ks);
        shader.setUniform1f("u_shiny", scene.table_material.shiny);

        const auto& tex = scene.textures[0];
        tex.bind(sampler_slot);
        node->mesh->draw();
        tex.unbind();
    }

    // Draw torus.
    {
        auto model = scene.torus_node->worldTransformation();
        shader.setUniformMat4f("u_model", model);
        shader.setUniformVec3f("u_ka", scene.torus_material.ka);
        shader.setUniformVec3f("u_kd", scene.torus_material.kd);
        shader.setUniformVec3f("u_ks", scene.torus_material.ks);
        shader.setUniform1f("u_shiny", scene.torus_material.shiny);

        const auto& tex = scene.textures[1];
        tex.bind(sampler_slot);
        scene.torus_node->mesh->draw();
        tex.unbind();
    }

    // Draw teapot.
    {
        auto model = scene.teapot_node->worldTransformation();
        shader.setUniformMat4f("u_model", model);
        shader.setUniformVec3f("u_ka", scene.teapot_material.ka);
        shader.setUniformVec3f("u_kd", scene.teapot_material.kd);
        shader.setUniformVec3f("u_ks", scene.teapot_material.ks);
        shader.setUniform1f("u_shiny", scene.teapot_material.shiny);

        const auto& tex = scene.textures[2];
        tex.bind(sampler_slot);
        scene.teapot_node->mesh->draw();
        tex.unbind();
    }

    // Draw icosahedron.
    {
        auto model = scene.ico_node->worldTransformation();
        shader.setUniformMat4f("u_model", model);
        shader.setUniformVec3f("u_ka", scene.ico_material.ka);
        shader.setUniformVec3f("u_kd", scene.ico_material.kd);
        shader.setUniformVec3f("u_ks", scene.ico_material.ks);
        shader.setUniform1f("u_shiny", scene.ico_material.shiny);

        const auto& tex = scene.textures[3];
        tex.bind(sampler_slot);
        scene.ico_node->mesh->draw();
        tex.unbind();
    }

    // Draw floor.
    {
        auto model = scene.floor_node->worldTransformation();
        shader.setUniformMat4f("u_model", model);
        shader.setUniformVec3f("u_ka", scene.floor_material.ka);
        shader.setUniformVec3f("u_kd", scene.floor_material.kd);
        shader.setUniformVec3f("u_ks", scene.floor_material.ks);
        shader.setUniform1f("u_shiny", scene.floor_material.shiny);

        const auto& tex = scene.textures[1];
        tex.bind(sampler_slot);
        scene.floor_node->mesh->draw();
        tex.unbind();
    }

    // Draw light source.
    {
        shader_light_source_.use();
        shader_light_source_.setUniformMat4f("u_view", camera.view());
        shader_light_source_.setUniformMat4f("u_projection", camera.projection());
        auto model = scene.point_light_node->worldTransformation();
        shader_light_source_.setUniformMat4f("u_model", model);
        scene.point_light_node->mesh->draw();
    }
}
