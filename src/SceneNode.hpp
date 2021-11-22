#ifndef SCENE_NODE_HPP
#define SCENE_NODE_HPP

#include <memory>    // for std::unique_ptr
#include <vector>

#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>

#include "Mesh.hpp"

// SceneNode represents the node of a scene tree.
struct SceneNode
{
public:

    SceneNode();
    SceneNode(Mesh* p_mesh);
    ~SceneNode();

    // Create a new node.
    SceneNode* makeSubnode();

    glm::mat4 localTransformation();
    glm::mat4 worldTransformation();

    glm::vec3 pos;
    glm::vec3 ori_x, ori_y, ori_z;
    glm::vec3 scale;

public:

    std::vector<std::unique_ptr<SceneNode>> subnodes;
    SceneNode* parent_node;
    Mesh* mesh;
};

#endif // SCENE_NODE_HPP
