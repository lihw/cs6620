/**
 * \file ray.hpp
 * \author lihw (lihw81@gmail.com)
 * \changelog
 * - 2019/09/03 initial check in
 *
 * The descriptions of nodes in the ray tracing scene.
 */


#ifndef RAY_HPP
#define RAY_HPP

#include "common.h"


CS6620_NAMESPACE_BEGIN

class Ray
{
public:
    vec3 origin;
    vec3 direction;
public:
    explicit Ray()
    {
        origin.Zero();
        direction = vec3(1.0f, 0.0f, 0.0f);
    }

    /**
     * Transform this ray to another space by the given transform.
     */
    inline void transform(const mat4& t)
    {
        origin = (t * this->origin).XYZ();
        direction = (t * this->direction).XYZ();
    }
    
    inline Ray getTransformed(const mat4 &t) const
    {
        Ray ret;
        
        ret.origin = (t * this->origin).XYZ();
        ret.direction = (t * this->direction).XYZ();
        
        return ret;
    }
};


CS6620_NAMESPACE_END


#endif // !RAY_HPP
