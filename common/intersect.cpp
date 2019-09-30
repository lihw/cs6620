/**
 * \file intersect.cpp
 * \author lihw (lihw81@gmail.com)
 * \changelog
 * - 2019/09/30 initial check in
 *
 * Compute the intersection of ray with geometries.
 */

#include "intersect.hpp"

#include "ray.hpp"

#include <algorithm>

CS6620_NAMESPACE_BEGIN

bool Intersect::ray_sphere(const Ray &ray, const vec3 position, f32 radius,
        vec3 *out_position, vec3 *out_normal)
{
    const f32 radius2 = radius * radius;
    vec3 L = position - ray.origin;
    f32 tca = L.Dot(ray.direction);
    // if (tca < 0) return false;
    f32 d2 = L.Dot(L) - tca * tca;
    if (d2 > radius2) 
    {
        return false;
    }
    f32 thc = sqrtf(radius2 - d2);
    f32 t0 = tca - thc;
    f32 t1 = tca + thc;

    if (t0 > t1) std::swap(t0, t1);

    if (t0 < 0) 
    {
        t0 = t1; // if t0 is negative, let's use t1 instead
        if (t0 < 0) 
        {
            return false; // both t0 and t1 are negative
        }
    }

    const f32 t = t0;

    *out_position = ray.origin + ray.direction * t;

    *out_normal = (*out_position - position).GetNormalized();

    return true;
}

CS6620_NAMESPACE_END


