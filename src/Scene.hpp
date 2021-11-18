#ifndef SCENE_HPP
#define SCENE_HPP

#include "SceneNode.hpp"

#include "VertexArray.hpp"

// TODO: Implement Scene class following the same abstractions of ASSIMP Scene.
// - https://learnopengl.com/Model-Loading/Assimp
// - http://assimp.sourceforge.net/lib_html/data.html
// - http://assimp.sourceforge.net/lib_html/structai_scene.html

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

private:

    // Root of scene tree structure.
    std::unique_ptr<SceneNode> root_;

    // Meshes used.
    VertexArray cube_;
    VertexArray square_;
    VertexArray icosahedron_;
    VertexArray torus_;
    VertexArray teapot_;
};

#endif // SCENE_HPP
