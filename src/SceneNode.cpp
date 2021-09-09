#include "SceneNode.hpp"

#include <utility>    // for std::move

#include "Math.hpp"

using namespace std;
using glm::vec3;
using glm::mat4;

SceneNode::SceneNode():
    pos(vec3(0.0f)),
    ori_x(vec3(1.0f, 0.0f, 0.0f)),
    ori_y(vec3(0.0f, 1.0f, 0.0f)),
    ori_z(vec3(0.0f, 0.0f, 1.0f)),
    scale(vec3(1.0f)),
    subnodes(),
    parent_node(nullptr),
    vertex_array(nullptr)
{
}

SceneNode::SceneNode(VertexArray* vao):
    SceneNode()
{
    assert(vao != nullptr);

    vertex_array = vao;
}

SceneNode::~SceneNode()
{
}

SceneNode* SceneNode::makeSubnode()
{
    subnodes.emplace_back(new SceneNode);
    SceneNode* new_node = subnodes.back().get();
    new_node->parent_node = this;

    return new_node;
}

//SceneNode* SceneNode::makeSubnode(SceneNode& node)
//{
//    subnodes.emplace_back(new SceneNode);
//    SceneNode* new_node = subnodes.back().get();
//
//    // Copy node data.
//    new_node->pos = node.pos;
//    new_node->ori_x = node.ori_x;
//    new_node->ori_y = node.ori_y;
//    new_node->ori_z = node.ori_z;
//    new_node->scale = node.scale;
//    new_node->parent_node = this;
//    new_node->vertex_array = node.vertex_array;
//    // Mode subnodes.
//    new_node->subnodes = move(node.subnodes);
//
//    return new_node;
//}

mat4 SceneNode::localTransformation()
{
    return getTranslation(pos) * getRotation(ori_x, ori_y, ori_z) * getScale(scale);
}

mat4 SceneNode::worldTransformation()
{
    mat4 world_transf = localTransformation();
    if (parent_node != nullptr) {
        world_transf = parent_node->worldTransformation() * world_transf;
    }
    return world_transf;
}
