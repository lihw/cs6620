/**
 * \file view.cpp
 * \author lihw (lihw81@gmail.com)
 * \changelog
 * - 2019/09/03 initial check in
 *
 * The preview of rendering result.
 */

#include "view.hpp"

#include "ppm.h"

#include <cassert>

CS6620_NAMESPACE_BEGIN

View::View(u32 width, u32 height)
{
    assert(width > 0 && height > 0);

    this->_width = width;
    this->_height = height;

    this->_image = new f32 [width * height * 3];
}
    
View::~View()
{
    delete [] this->_image;
}

bool View::dump(const char *outputFilePath) const noexcept 
{
    u8 *imageRGB8 = new u8 [this->_width * this->_height * 3];
    CvtRgb32f2Rgb8(this->_image, this->_width, this->_height, imageRGB8);
    bool ret = WritePPM(outputFilePath, this->_width, this->_height, imageRGB8);
    delete [] imageRGB8;
    return ret;
}
    
void View::write(const vec2u coordinate, const vec3 &color)
{
    u32 offset = (coordinate.y * this->_width + coordinate.x) * 3;
    f32 *p = &this->_image[offset];
    p[0] = color.x;
    p[1] = color.y;
    p[2] = color.z;
}

void CvtRgb32f2Rgb8(const float *rgb32f, u32 width, u32 height, u8 *rgb8)
{
    for (u32 i = 0; i < height; i++)
    for (u32 j = 0; j < width; j++)
    {
        const float *src = &rgb32f[(i * width + j) * 3];
        u8 *dst = &rgb8[(i * width + j) * 3];

        dst[0] = (u8)(src[0] * 255.0f);
        dst[1] = (u8)(src[1] * 255.0f);
        dst[2] = (u8)(src[2] * 255.0f);
     }
}

CS6620_NAMESPACE_END

