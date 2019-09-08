/**
 * \file tree.cpp
 * \author lihw (lihw81@gmail.com)
 * \changelog
 * - 2019/09/08 initial check in
 *
 * The ray-node intersection acceleration structure.
 * The base class also provides a naive solution.
 */

#include "tree.hpp"

#include "scene_node.hpp"
#include "scene.hpp"

#include <list>

CS6620_NAMESPACE_BEGIN

Tree::Tree(Scene *scene)
{
    // Extract the nodes from the scene and store them
    // into a flat array.
    std::list<SceneNode *> nodes(scene->root->children.begin(), scene->root->children.end());

    while (!nodes.empty())
    {
        SceneNode *node = nodes.front();
        nodes.pop_front();

        nodes.insert(nodes.end(), node->children.begin(), node->children.end());

        this->_nodes.push_back(node);
    }
}
    
Tree::~Tree()
{
}

bool Tree::intersect(const Ray &ray, SceneNode *out_node, vec3 &out_position, vec3 &out_normal) noexcept
{
    for (auto &&node : this->_nodes)
    {
        GeometricNode *gnode = reinterpret_cast<GeometricNode *>(node);
        if (gnode->intersect(ray, out_position, out_normal))
        {
            out_node = node;
            return true;
        }
    }

    return false;
}


CS6620_NAMESPACE_END
