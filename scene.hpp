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

#include "cyMatrix.h"

#include <vector>
#include <string>

CS6620_BEGIN_NAMESPACE

/**
 * The base class of scene nodes.
 */
class SceneNode 
{
public:
    enum class Type
    {
        UNKNOWN,        /**< Unknown. */
        GEOMETRY,       /**< A geometric object. */
        CAMERA,         /**< A camera object. */
    } type;       

    std::string name;

    f32       scaling;        /**< Local transformation. */
    Vec3<f32> translation;
    Vec3<f32> rotation;

    Matrix4 transform; /**< Local transformation matrix. */
    Matrix4 globalTransform; /**< The global transformation matrix. */

    SceneNode               *parent    = nullptr;
    std::vector<SceneNode *> children;

public:
    /**
     * Constructor
     */
    SceneNode(const char *name);
    /**
     * Destructor
     */
    virtual ~SceneNode();
    /**
     * Load the content of this node from its xml description.
     */
    virtual bool unserialize(XmlElement *xmlElement) noexcept = 0;
    /**
     * Convert the node's data into a xml description.
     */
    //virtual const char * serialize() noexcept = 0;
};

/**
 * Geometric scnee node.
 */
class GeometriceNode : public SceneNode
{
public:

public:
    /**
     */
    GeometricNode();
    /**
     */
    virtual ~GeometricNode();
    /**
     */
    virtual bool unserialize(XmlElement *xmlElement) noexcept override = 0;

protected:
    void _parseScale(XmlElement *xmlElement);
    void _parseTranslate(XmlElement *xmlElement);
    void _parseRotate(XmlElement *xmlElement);
};

/**
 */

/**
 * Camera node.
 */
class Camera 
{
public:
    f32 position[3];
    f32 target[3];
    f32 up[3];
    f32 fovy;
    u16 width;
    u16 height;

public:
    /**
     */
    Camera();
    /**
     */
    virtual ~Camera();
    /**
     */
    virtual bool load() noexcept override;
};

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

private:
    std::vector<SceneNode *> _nodes; /**< The scene nodes arranged in a flattened array. */
    SceneNode               *_root;  /**< The scene node. */
    Camera                  *_camera; /**< The scene camera. */
};


CS6620_END_NAMESPACE


#endif // !SCENE_HPP
