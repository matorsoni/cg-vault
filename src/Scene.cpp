#include "Scene.hpp"

#include "Geometry.hpp"
#include "Teapot.hpp"

using namespace std;
using glm::vec3;
using glm::mat4;


TableScene::TableScene():
    root_(make_unique<SceneNode>()),
    // Meshes.
    cube_(createCubeWithoutIndices()),
    square_(createSquare()),
    sphere_(createSphere(30, 30)),
    torus_(createTorus(1.0f, 0.15f)),
    teapot_(createTeapot(2.0f)),
    // Objects.
    table_node(nullptr),
    sphere_node(nullptr),
    torus_node(nullptr),
    teapot_node(nullptr),
    point_light_node(nullptr)
{
    assert(root_);

    // Load textures.
    textures.emplace_back("../assets/wood0.jpeg");
    textures.emplace_back("../assets/wood1.jpeg");
    textures.emplace_back("../assets/chess.jpeg");
    textures.emplace_back("../assets/psycho1.jpeg");

    // Change a few material parameters.
    table_material.ka = vec3(128.f, 83.f, 0.f) / 255.f;
    table_material.kd = vec3(181.f, 88.f, 0.f) / 255.f;
    sphere_material.ka = vec3(0.1f, 0.1f, 0.6f);
    teapot_material.shiny = 200.f;

    // Push mesh data to GPU.
    cube_.pushToGpu();
    square_.pushToGpu();
    sphere_.pushToGpu();
    torus_.pushToGpu();
    teapot_.pushToGpu();

    // Table variables.
    const float table_length = 2.0f;
    const float table_width = 1.0f;
    const float leg_height = 1.0f;
    const float leg_width = 0.1f;
    const float top_height = 0.1f;
    const float top_scale_factor = 1.1f;

    // Table object.
    table_node = root_->makeSubnode();
    // Legs.
    {
        const vec3 leg_scale = vec3(leg_width, leg_height, leg_width);
        vec3 leg_position[] = {
            vec3(table_width/2, leg_height/2, table_length/2),
            vec3(table_width/2, leg_height/2, -table_length/2),
            vec3(-table_width/2, leg_height/2, -table_length/2),
            vec3(-table_width/2, leg_height/2, table_length/2)
        };
        for (int i = 0; i < 4; ++i) {
            SceneNode* leg_object = table_node->makeSubnode();
            leg_object->pos = leg_position[i];
            leg_object->scale = leg_scale;
            leg_object->mesh = &cube_;
        }
    }
    // Top.
    {
        SceneNode* top_object = table_node->makeSubnode();
        top_object->pos = vec3(0.0f, leg_height + top_height/2, 0.0f);
        top_object->scale = vec3(top_scale_factor * table_width,
                                 top_height,
                                 top_scale_factor * table_length);
        top_object->mesh = &cube_;
    }

    const float table_top_y = leg_height + top_height;

    // Sphere object.
    sphere_node = root_->makeSubnode();
    sphere_node->pos = vec3{0.0f, table_top_y + 0.8f, -0.5f};
    sphere_node->scale = vec3(0.35f);
    sphere_node->mesh = &sphere_;

    // Torus object.
    torus_node = root_->makeSubnode();
    torus_node->pos = sphere_node->pos;
    torus_node->scale = vec3(0.5f);
    torus_node->mesh = &torus_;

    // Teapot object.
    teapot_node = root_->makeSubnode();
    teapot_node->pos = vec3(0.0f, table_top_y + 0.01f, 0.65f);
    teapot_node->ori_y = vec3(0.0f, 0.0f, -1.0f);
    teapot_node->ori_z = cross(teapot_node->ori_x, teapot_node->ori_y);
    teapot_node->scale = vec3(0.2f);
    teapot_node->mesh = &teapot_;

    // Floor plane object.
    floor_node = root_->makeSubnode();
    floor_node->pos = vec3(0.0f);
    floor_node->scale = vec3(3.5f);
    floor_node->mesh = &square_;

    // Light source.
    point_light_node = root_->makeSubnode();
    point_light_node->pos = vec3{0.0f, 2.0f, 0.0f};
    point_light_node->scale = vec3(0.1f);
    point_light_node->mesh = &cube_;
}

SceneNode* TableScene::root() const
{
    return root_.get();
}
