/**
 * \file view.hpp
 * \author lihw (lihw81@gmail.com)
 * \changelog
 * - 2019/09/03 initial check in
 *
 * The preview of rendering result.
 */

#ifndef VIEW_HPP
#define VIEW_HPP

#include "common.h"

CS6620_BEGIN_NAMESPACE

/**
 * The preview window of the rendering result.
 */
class View
{
public:
    /**
     */
    explicit View(u32 width, u32 height);
    /**
     */
    ~View();
    /**
     * Dump the current result in memory to the disk.
     */
    bool dump(const char *outputFilePath) noexcept const;
    /**
     * Write the color to the memory in specific coordinate.
     * @param coordinate The image pixel coordinate.
     * @param color The color value in floating RGBA format.
     */
    void write(const vec2u coordinate, const vec4 &color);

private:
    f32 *_image = nullptr; /**< The image in memory (width x height x rgba). TODO: resolved? */
    u32 _width;
    u32 _height;
};

extern void CvtRgba32f2Rgb8(const float *rgba32f, u32 width, u32 height, u8 *rgb8);

CS6620_END_NAMESPACE


#endif // !VIEW_HPP
