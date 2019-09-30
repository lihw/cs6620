/**
 * \file intersect.hpp
 * \author lihw (lihw81@gmail.com)
 * \changelog
 * - 2019/09/30 initial check in
 *
 * Compute the intersection of ray with geometries. 
 */

#ifndef INTERSECT_HPP
#define INTERSECT_HPP

#include "common.h"

CS6620_NAMESPACE_BEGIN

class Ray;

class Intersect
{
public:
    static bool ray_sphere(const Ray &ray, const vec3 position, f32 radius,
        vec3 *out_position, vec3 *out_normal);
};

CS6620_NAMESPACE_END


#endif // !INTERSECT_HPP
