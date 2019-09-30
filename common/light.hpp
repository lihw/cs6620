/**
 * \file scene_node.hpp
 * \author lihw (lihw81@gmail.com)
 * \changelog
 * - 2019/09/03 initial check in
 *
 * The descriptions of nodes in the ray tracing scene.
 */

#ifndef LIGHT_HPP
#define LIGHT_HPP

#include "common.h"

#include <vector>
#include <string>

#include "tinyxml2.h"
#include "cyMatrix.h"
#include "cyVector.h"

#include "scene_node.hpp"

CS6620_NAMESPACE_BEGIN

class Light : public SceneNode
{
public:
    f32 intensity;  /**< The light intensity. */
    union
    {
        vec3 direction; /**< The light direction, valid for directional lights. */
        vec3 position;  /**< The light position,  valid for point lights. */
    };
    enum 
    {
        AMBIENT,
        DIRECT,
        POINT,
    } type;
public:
    /**
     */
    Light(const char *name, SceneNode *parent);
    /**
     */
    virtual ~Light();
    /**
     * Read the parameter of the light from xml doc.
     */
    virtual bool unserialize(tinyxml2::XMLElement *xmlElement) noexcept override;

    /**
     * Fetch the incident light direction.
     */
    vec3 incident(const vec3 &position) const;
private:
    /**
     */
    void _parseIntensity(tinyxml2::XMLElement *xmlElement);
    /**
     */
    void _parseDirection(tinyxml2::XMLElement *xmlElement);
    /**
     */
    void _parsePosition(tinyxml2::XMLElement *xmlElement);
};



CS6620_NAMESPACE_END


#endif // !LIGHT_HPP
