/**
 * \file material.hpp
 * \author lihw (lihw81@gmail.com)
 * \changelog
 * - 2019/09/15 initial check in
 *
 * The materials.
 */

#ifndef MATERIAL_HPP
#define MATERIAL_HPP

#include "common.h"

#include <vector>
#include <string>

#include "tinyxml2.h"
#include "cyMatrix.h"
#include "cyVector.h"

#include "scene_node.hpp"

CS6620_NAMESPACE_BEGIN

/**
 * The base class of scene nodes.
 */
class Material : public SceneNode
{
public:
    std::string type;

public:
    /**
     * Constructor
     */
    Material(const char *name, SceneNode *parent);
    /**
     * Destructor
     */
    virtual ~Material();
    /**
     * Load the content of this node from its xml description.
     */
    virtual bool unserialize(tinyxml2::XMLElement *xmlElement) noexcept;
    /**
     * Compute the shading color with the material properties.
     */
    virtual vec3 shade(const vec3 &incident, vec3 &reflection, vec3 &normal) const noexcept;
};

/**
 * Geometric scnee node.
 */
class BlinnMaterial : public MaterialNode
{
public:
    /**
     * Constructor.
     */
    BlinnMaterial(const char *name, SceneNode *parent);
    /**
     * Destructor.
     */
    virtual ~BlinnMaterial();
    /**
     * Parse the node's attributes from xml document.
     * @param xmlElement the XML node that contains information of this node.
     */
    virtual bool unserialize(tinyxml2::XMLElement *xmlElement) noexcept override;
    /**
     * Compute the shading color with the material properties.
     */
    virtual vec3 shade(const vec3 &incident, vec3 &reflection, vec3 &normal) const noexcept;

protected:
    /**
     */
    void _parseSpecular(tinyxml2::XMLElement *xmlElement);
    /**
     */
    void _parseDiffuse(tinyxml2::XMLElement *xmlElement);
};


CS6620_NAMESPACE_END


#endif // !MATERIAL_HPP
