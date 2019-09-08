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

        Ray *operator->()
        {
            return &this->_ray;
        }

        /**
         * Prefix increment.
         */
        RayIterator &operator++() 
        {
            if (++this->_x >= this->_camera->width)
            {
                this->_x = 0;
                this->_y++;
            }
            
            this->_updateRay();

            return *this;
        }

        void _updateRay()
        {
            this->_ray.origin = this->_camera->position;

            auto xx = ((f32)this->_x + 0.5f) / (f32)this->_camera->width * 2.0f - 1.0f;
            auto yy = ((f32)(this->_camera->height - 1 - this->_y) + 0.5f) / (f32)this->_camera->height * 2.0f - 1.0f;
            
            vec3 sample = this->_camera->nearo + 
                this->_camera->nearx * xx + this->_camera->nearz * yy;

            this->_ray.direction = sample - this->_ray.origin;
            this->_ray.direction.Normalize();
        }

        bool operator==(const RayIterator &ri) const
        {
            return this->_x == ri._x && this->_y == ri._y;
        }

        RayIterator &operator=(const RayIterator &other)
        {
            this->_camera = other._camera;
            this->_x = other._x;
            this->_y = other._y;

            this->_updateRay();

            return *this;
        }

        bool operator!=(const RayIterator &ri) const 
        {
            return !(this->_x == ri._x && this->_y == ri._y);
        }

        vec2u coordinate() const
        {
            return vec2u(this->_x, this->_y);
        }
        
    private:
        Camera *_camera;
        u32     _x;     /**< The current pixel. */
        u32     _y;     /**< Ditto. */
        Ray     _ray;   /**< The current ray. */
    };

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
     * Get the first ray
     */
    RayIterator beginRay();
    /**
     * The last ray.
     */
    RayIterator endRay();

private:
};


CS6620_NAMESPACE_END


#endif // !CAMERA_HPP
