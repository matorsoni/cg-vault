#include "SceneNode.hpp"

using namespace std;
using glm::vec3;

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

SceneNode* SceneNode::make_subnode()
{
    subnodes.emplace_back(new SceneNode);
    SceneNode* new_node = subnodes.back().get();
    new_node->parent_node = this;
    return new_node;
}
