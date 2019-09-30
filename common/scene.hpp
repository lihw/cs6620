/**
 * \file scene.hpp
 * \author lihw (lihw81@gmail.com)
 * \changelog
 * - 2019/09/01 initial check in
 *
 * The description of a ray tracing scene.
 */

#ifndef SCENE_HPP
#define SCENE_HPP

#include "common.h"

#include <vector>
#include <string>

#include "tinyxml2.h"
#include "cyMatrix.h"
#include "cyVector.h"

CS6620_NAMESPACE_BEGIN

class Camera;
class SceneNode;
class Tree;
class Ray;
class Light;
class Material;

/**
 * The world space is z-up
 */
class Scene 
{
public:
    Camera    *camera = nullptr; /**< The scene camera. */
    SceneNode *root   = nullptr;  /**< The scene node. */
public:
    /**
     * Constructor
     */
    explicit Scene();
    /**
     * Destructor
     * Clear and destroy all the scene information.
     */
    virtual ~Scene();
    /**
     * Load the scene from .xml file.
     * @param sceneFile the scene file path.
     * @return true if load succeeds.
     */
    bool load(const char *sceneFile) noexcept;

    /**
     * Pre-process the scene for following path tracing, e.g., build accelerate
     * data structure for intersection.
     */
    void prepare() noexcept;

    /**
     * Compute the result color of the ray shooting from image plane.
     */
    vec3 shade(const Ray &ray);
protected:
    /**
     * Destroy the scene.
     */
    void _destroy();

private:
    Tree *_tree = nullptr; /**< The intersection acceleration object. */
    std::vector<Light *> _lights;      /**< The lights in the scene. */
    std::vector<Material *> _materials;/**< The materials used in the scene. */
};


CS6620_NAMESPACE_END


#endif // !SCENE_HPP
