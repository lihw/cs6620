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

#include <glog/logging.h>

typedef float f32;
typedef unsigned short u16;
typedef unsigned int u32;
typedef int i32;
typedef short i16;
typedef char i8;
typedef unsigned char u8;

#define CS6620_NAMESPACE_BEGIN namespace cs6620 {
#define CS6620_NAMESPACE_END };

typedef cy::Vec4<f32> vec4;
typedef cy::Vec3<f32> vec3;
typedef cy::Vec2<f32> vec2;
typedef cy::Matrix4<f32> mat4;
typedef cy::Vec2<u32> vec2u;

//#define M_PI 3.11415926f


#endif // !COMMON_H
