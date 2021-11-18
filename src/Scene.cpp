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
    icosahedron_(createSubdividedIcosahedron(3)),
    torus_(createTorus(1.0f, 0.15f)),
    teapot_(createTeapot(2.0f)),
    // Objects.
    table_node(nullptr),
    ico_node(nullptr),
    torus_node(nullptr),
    teapot_node(nullptr),
    point_light_node(nullptr)
{
    assert(root_);

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
            leg_object->vertex_array = &cube_;
        }
    }
    // Top.
    {
        SceneNode* top_object = table_node->makeSubnode();
        top_object->pos = vec3(0.0f, leg_height + top_height/2, 0.0f);
        top_object->scale = vec3(top_scale_factor * table_width,
                                 top_height,
                                 top_scale_factor * table_length);
        top_object->vertex_array = &cube_;
    }

    const float table_top_y = leg_height + top_height;

    // Icosahedron object.
    ico_node = root_->makeSubnode();
    ico_node->pos = vec3{0.0f, table_top_y + 0.8f, -0.5f};
    ico_node->scale = vec3(0.35f);
    ico_node->vertex_array = &icosahedron_;

    // Torus object.
    torus_node = root_->makeSubnode();
    torus_node->pos = ico_node->pos;
    torus_node->scale = vec3(0.5f);
    torus_node->vertex_array = &torus_;

    // Teapot object.
    teapot_node = root_->makeSubnode();
    teapot_node->pos = vec3(0.0f, table_top_y + 0.01f, 0.65f);
    teapot_node->ori_y = vec3(0.0f, 0.0f, -1.0f);
    teapot_node->ori_z = cross(teapot_node->ori_x, teapot_node->ori_y);
    teapot_node->scale = vec3(0.2f);
    teapot_node->vertex_array = &teapot_;

    // Light source.
    point_light_node = root_->makeSubnode();
    point_light_node->pos = vec3{0.0f, 2.0f, 0.0f};
    point_light_node->scale = vec3(0.1f);
    point_light_node->vertex_array = &cube_;
}

SceneNode* TableScene::root() const
{
    return root_.get();
}