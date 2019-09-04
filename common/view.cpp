/**
 * \file view.cpp
 * \author lihw (lihw81@gmail.com)
 * \changelog
 * - 2019/09/03 initial check in
 *
 * The preview of rendering result.
 */

#include "view.hpp"

#include <cassert>

CS6620_BEGIN_NAMESPACE

View::View(u32 width, u32 height)
{
    assert(width > 0 && height > 0);

    this->_width = width;
    this->_height = height;

    this->_image = new f32 [width * height * 4];
}
    
View::~View()
{
    delete [] this->_image;
}

bool View::dump(const char *outputFilePath) noexcept const
{
    return true;
}
    
void View::write(const vec2u coordinate, const vec4 &color)
{
    u32 offset = (coordinate.y * this->_width + coordinate.x) * 4;
    f32 *p = &this->_image[offset];
    p[0] = color.x;
    p[1] = color.y;
    p[2] = color.z;
    p[3] = color.w;
}


CS6620_END_NAMESPACE


#endif // !VIEW_HPP
