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

#include "cvVector.h"

CS6620_BEGIN_NAMESPACE

struct Ray
{
    vec3 origin;
    vec3 direction;

    explicit Ray()
    {
        origin.Zero();
        direction = vec3(1.0f, 0.0f, 0.0f);
    }
};


CS6620_END_NAMESPACE


#endif // !RAY_HPP
