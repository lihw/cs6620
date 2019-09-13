/**
 * \file camera.hpp
 * \author lihw (lihw81@gmail.com)
 * \changelog
 * - 2019/09/03 initial check in
 *
 * The description of a camera in the ray tracing scene.
 */

#ifndef CAMERA_HPP
#define CAMERA_HPP

#include "common.h"

#include <vector>
#include <string>

#include "tinyxml2.h"

#include "ray.hpp"

CS6620_NAMESPACE_BEGIN


/**
 * Camera node.
 */
class Camera 
{
public:
    vec3 position;
    vec3 target;
    vec3 up;
    f32 fovy;
    u16 width;   /**< The resolution. The default values are 512x512.  */
    u16 height;  /**< Ditto */

    vec3 nearo; /**< The near plane origin in world space. */
    vec3 nearx; /**< The near plane x axis direction. */
    vec3 nearz; /**< The near plane z axis direction. */

public:
    /**
     */
    Camera();
    /**
     */
    virtual ~Camera();
    /**
     */
    virtual bool unserialize(tinyxml2::XMLElement *xmlElement) noexcept;

    /**
     * Project a screen position in to a ray.
     */
    Ray unproject(f32 x, f32 y) const;
};


CS6620_NAMESPACE_END


#endif // !CAMERA_HPP
