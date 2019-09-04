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
#include "cvMatrix.h"
#include "cvVector.h"

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

    f32   scale;        /**< Local transformation. */
    vec3 translate;
    vec3 rotate;

    mat4f transform; /**< Local transformation matrix. */
    mat4f globalTransform; /**< The global transformation matrix. */

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
    /**
     * Constructor.
     */
    GeometricNode();
    /**
     * Destructor.
     */
    virtual ~GeometricNode();
    /**
     * Parse the node's attributes from xml document.
     * @param xmlElement the XML node that contains information of this node.
     */
    virtual bool unserialize(XmlElement *xmlElement) noexcept override;
    /**
     * If intersect with a given ray.
     */
    virtual bool intersect(const Ray &ray) noexcept override = 0;

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
    void _parseScale(XmlElement *xmlElement);
    /**
     */
    void _parseTranslate(XmlElement *xmlElement);
    /**
     */
    void _parseRotate(XmlElement *xmlElement);
};

/**
 */
class GeometricSphereNode : public GeometricNode
{
public:
    /**
     */
    GeometricSphereNode();
    /**
     */
    virtual ~GeometricSphereNode();
    /**
     * Read the parameter of the sphere node from xml doc.
     */
    virtual bool unserialize(XmlElement *xmlElement) noexcept override;
    /**
     * If intersect with a given ray in world space.
     */
    virtual bool intersect(const Ray &ray) noexcept override;
protected:
    /**
     * Get the radius of the sphere.
     */
    inline f32 _radius() const { return this->_radius; };
    /**
     * Get the position of the sphere.
     */
    inline vec3  position() const { return this->_position  };
    
protected:
    vec3 _position; /**< the coordinate of the center. */
    f32  _radius;   /**< The radius. */
};


CS6620_END_NAMESPACE


#endif // !SCENE_NODE_HPP
