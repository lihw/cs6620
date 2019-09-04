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
#include "cvMatrix.h"
#include "cvVector.h"

CS6620_BEGIN_NAMESPACE

class Scene 
{
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

protected:
    /**
     * Destroy the scene.
     */
    void _destroy();

private:
    std::vector<SceneNode *> _nodes; /**< The scene nodes arranged in a flattened array. */
    SceneNode               *_root   = nullptr;  /**< The scene node. */
    Camera                  *_camera = nullptr; /**< The scene camera. */
};


CS6620_END_NAMESPACE


#endif // !SCENE_HPP
