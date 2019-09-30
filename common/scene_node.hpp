/**
 * \file scene_node.hpp
 * \author lihw (lihw81@gmail.com)
 * \changelog
 * - 2019/09/03 initial check in
 *
 * The descriptions of nodes in the ray tracing scene.
 */

#ifndef SCENE_NODE_HPP
#define SCENE_NODE_HPP

#include "common.h"

#include <vector>
#include <string>

#include "tinyxml2.h"
#include "cyMatrix.h"
#include "cyVector.h"

#include "ray.hpp"

CS6620_NAMESPACE_BEGIN

class Material;

/**
 * The base class of scene nodes.
 */
class SceneNode 
{
public:
    enum class Category
    {
        UNKNOWN,        /**< Unknown. */
        ROOT,           /**< The scene root. */
        GEOMETRY,       /**< A geometric object. */
    } category;       

    std::string name;

    vec3 scale;        /**< Local transformation. */
    vec3 translate;
    vec3 rotate;

    mat4 transform; /**< Local transformation matrix. */
    mat4 globalTransform; /**< The global transformation matrix. */
    mat4 invGlobalTransform; /**< The inverse of global transform matrix. */

    SceneNode               *parent    = nullptr;
    std::vector<SceneNode *> children;

    std::string  materialName = "";
    Material    *material = nullptr; /**< The material object of this node. */

public:
    /**
     * Constructor
     */
    SceneNode(const char *name, SceneNode *parent);
    /**
     * Destructor
     */
    virtual ~SceneNode();
    /**
     * Load the content of this node from its xml description.
     */
    virtual bool unserialize(tinyxml2::XMLElement *xmlElement) noexcept;
    /**
     * Convert the node's data into a xml description.
     */
    //virtual const char * serialize() noexcept = 0;
};

/**
 * Geometric scnee node.
 */
class GeometricNode : public SceneNode
{
public:
    /**
     * Constructor.
     */
    GeometricNode(const char *name, SceneNode *parent);
    /**
     * Destructor.
     */
    virtual ~GeometricNode();
    /**
     * Parse the node's attributes from xml document.
     * @param xmlElement the XML node that contains information of this node.
     */
    virtual bool unserialize(tinyxml2::XMLElement *xmlElement) noexcept override;
    /**
     * If intersect with a given ray.
     */
    virtual bool intersect(const Ray &ray, vec3 &out_position, vec3 &out_normal) noexcept = 0;

protected:
    /**
     * Update the local transform from SRT vectors.
     */
    void _updateTransform();
    /**
     * Update the global transform from SRT vectors.
     */
    void _updateGlobalTransform();
    /**
     */
    void _parseScale(tinyxml2::XMLElement *xmlElement);
    /**
     */
    void _parseTranslate(tinyxml2::XMLElement *xmlElement);
    /**
     */
    void _parseRotate(tinyxml2::XMLElement *xmlElement);

protected:
    Material *_material = nullptr;
};

/**
 * A sphere defined by position and radius.
 */
class GeometricSphereNode : public GeometricNode
{
public:
    /**
     */
    GeometricSphereNode(const char *name, SceneNode *parent);
    /**
     */
    virtual ~GeometricSphereNode();
    /**
     * Read the parameter of the sphere node from xml doc.
     */
    virtual bool unserialize(tinyxml2::XMLElement *xmlElement) noexcept override;
    /**
     * If intersect with a given ray in world space.
     */
    virtual bool intersect(const Ray &ray, vec3 &out_position, vec3 &out_normal) noexcept override;

private:
    f32 _radius; /**< The radius of the sphere. */
    vec3 _position; /**< The position of the sphere center in world space. */
};

class SceneNodeFactory 
{
public:
    /**
     * Constructor.
     */
    SceneNodeFactory();
    /**
     * Destructor.
     */
    ~SceneNodeFactory();

    static SceneNode *unserialize(tinyxml2::XMLElement *xmlElement, SceneNode *parent) noexcept;
};


CS6620_NAMESPACE_END


#endif // !SCENE_NODE_HPP
