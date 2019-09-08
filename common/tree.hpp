/**
 * \file tree.hpp
 * \author lihw (lihw81@gmail.com)
 * \changelog
 * - 2019/09/08 initial check in
 *
 * The ray-node intersection acceleration structure.
 * The base class also provides a naive solution.
 */

#ifndef TREE_HPP
#define TREE_HPP

#include "common.h"

#include <vector>

CS6620_NAMESPACE_BEGIN

class Scene;
class SceneNode;
class Ray;


class Tree
{
public:
    /**
     * Constructor.
     */
    explicit Tree(Scene *scene);
    /**
     */
    virtual ~Tree();

    /**
     * Compute the intersection of a ray with the scene.
     * @param ray the ray in world space.
     * @param out_node return the nearest intersecting node.
     * @param out_position return the intersection in world coordinate.
     * @param out_normal return the normal at the intersection point.
     * @return return true if the ray intersection happens.
     */
    bool intersect(const Ray &ray, SceneNode *out_node, vec3 &out_position, vec3 &out_normal) noexcept;

private:
    std::vector<SceneNode *> _nodes; /**< The nodes of the scene in a flat array .*/
};

CS6620_NAMESPACE_END


#endif // !TREE_HPP
