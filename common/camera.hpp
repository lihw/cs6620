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
#include "cvMatrix.h"
#include "cvVector.h"

CS6620_BEGIN_NAMESPACE

class RayIterator
{
    friend class Camera;

public:
    explicit RayIterator(Camera *camera)
    {
        this->_camera = camera;
    }

    Ray &operator*()
    {
        return this->_ray;
    }

    void operator++(void)
    {
        if (++this->_x >= this->_camera.width)
        {
            this->_x = 0;
            this->_y++;
        }

        this->_ray.origin = this->_camera->position;

        f32 invx = 1.0f / (f32)this->_camera->width;
        f32 invy = 1.0f / (f32)this->_camera->height;

        auto xx = ((f32)this->_x + 0.5f) / invx * 2.0f - 1.0f;
        auto yy = ((f32)(this->_camera->height - 1 - this->_y) + 0.5f) / invy * 2.0f - 1.0f;
        
        vec3 sample = this->_camera.center + 
            this->_camera.scalex * xx + this->_camera.scaley * yy;

        this->_ray.direction = sample - this->_ray.origin;
        this->_ray.direction.Normalize();
    }

    bool operator==(const RayIterator &ri) const
    {
        return this->_x == ri._x && this->_y == ri._y;
    }
    
private:
    Camera *_camera;
    u32     _x;     /**< The current pixel. */
    u32     _y;     /**< Ditto. */
    Ray     _ray;   /**< The current ray. */
};

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
    vec3 nearx; /**< The near plane x direction. */
    vec3 neary; /**< The near plane y direction. */
public:
    /**
     */
    Camera();
    /**
     */
    virtual ~Camera();
    /**
     */
    virtual bool unserialize(XmlElement *xmlElement) noexcept override = 0;
    /**
     * Get the first ray
     */
    RayIterator beginRay();
    /**
     * The last ray.
     */
    RayIterator endRay();

private:
};


CS6620_END_NAMESPACE


#endif // !CAMERA_HPP
