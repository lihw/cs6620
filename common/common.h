/**
 * \file common.h
 * \author lihw (lihw81@gmail.com)
 * \changelog
 * - 2019/09/01 initial check in
 *
 * The common definitions for the ray tracer.
 */

#ifndef COMMON_H
#define COMMON_H

#include "cyMatrix.h"
#include "cyVector.h"

typedef float f32;
typedef unsigned short u16;
typedef unsigned int u32;
typedef int i32;
typedef short i16;
typedef char i8;
typedef unsigned char u8;

#define CS6620_NAMESPACE_BEGIN namespace cs6620 {
#defined CS6620_NAMESPACE_END };

typedef Vec4<f32> vec4;
typedef Vec3<f32> vec3;
typedef Vec2<f32> vec2;
typedef Matrix4<f32> mat4;

#endif // !COMMON_H
