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
};


CS6620_NAMESPACE_END


#endif // !RAY_HPP
