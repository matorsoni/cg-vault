#ifndef SCENE_HPP
#define SCENE_HPP

#include <glm/vec3.hpp>

#include "SceneNode.hpp"

#include "Mesh.hpp"

// TODO: Implement Scene class following the same abstractions of ASSIMP Scene.
// - https://learnopengl.com/Model-Loading/Assimp
// - http://assimp.sourceforge.net/lib_html/data.html
// - http://assimp.sourceforge.net/lib_html/structai_scene.html

// Simple phong material parameters.
struct PhongMaterial
{
    glm::vec3 ka{0.0f, 0.0f, 0.0f};
    glm::vec3 kd{0.0f, 1.0f, 0.0f};
    glm::vec3 ks{1.0f, 1.0f, 1.0f};
    float shiny{10.0f};
};


class TableScene
{
public:

    TableScene();

    SceneNode* root() const;

    // Scene objects.
    SceneNode* table_node;
    SceneNode* ico_node;
    SceneNode* torus_node;
    SceneNode* teapot_node;
    SceneNode* point_light_node;

    // Phong materials.
    PhongMaterial ico_material;
    PhongMaterial torus_material;
    PhongMaterial table_material;
    PhongMaterial teapot_material;

private:

    // Root of scene tree structure.
    std::unique_ptr<SceneNode> root_;

    // Meshes used.
    Mesh cube_;
    Mesh square_;
    Mesh icosahedron_;
    Mesh torus_;
    Mesh teapot_;
};

#endif // SCENE_HPP
