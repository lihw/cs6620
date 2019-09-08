/**
 * \file shader.h
 * \author lihw (lihw81@gmail.com)
 * \changelog
 * - 2019/09/01 initial check in
 *
 * Shade a surface point with geometric, material properities and lighting.
 */

#ifndef SHADER_HPP
#define SHADER_HPP

#include "common.h"

CS6620_NAMESPACE_BEGIN

class SceneNode;
class Light;

class Shader 
{
public:
    /**
     * Constructor.
     */
    explicit Shader();
    /**
     * Destructor.
     */
    virtual ~Shader();
    /**
     * The shading function, i.e, BRDF.
     */
    vec3 shade(SceneNode *node, vec3 normal, vec3 normal, Light *lights, u32 numLights);
}

CS6620_NAMESPACE_END

#endif
