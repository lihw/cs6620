/**
 * \file camera.cpp
 * \author lihw (lihw81@gmail.com)
 * \changelog
 * - 2019/09/03 initial check in
 *
 * The description of the camera in a ray tracing scene.
 */

#include "camera.hpp"

#include "view.hpp"

CS6620_NAMESPACE_BEGIN

Camera::Camera()
{
    this->position = vec3(0.0f, 0.0f, 0.0f);
    this->target   = vec3(0.0f, 0.0f, 0.0f);
    this->up       = vec3(0.0f, 1.0f, 0.0f);
    this->fovy     = 45.0f;
    this->width    = 512;
    this->height   = 512;
}

Camera::~Camera()
{
}

bool Camera::unserialize(tinyxml2::XMLElement *xmlElement) noexcept 
{
    tinyxml2::XMLElement *childElement = xmlElement->FirstChildElement();

    bool seenPosition = false;
    bool seenTarget = false;
    bool seenFov = false;
    bool seenWidth = false;
    bool seenHeight = false;
    bool seenUp = false;

    // Parse the element and extract properties of the camera.
    while (childElement != nullptr)
    {
        const char *tagName = childElement->Name();
        if (strncmp(tagName, "position", 8) == 0)
        {
            this->position.x = childElement->FloatAttribute("x");
            this->position.y = childElement->FloatAttribute("y");
            this->position.z = childElement->FloatAttribute("z");

            seenPosition = true;
        }
        else if (strncmp(tagName, "target", 6) == 0)
        {
            this->target.x = childElement->FloatAttribute("x");
            this->target.y = childElement->FloatAttribute("y");
            this->target.z = childElement->FloatAttribute("z");
            
            seenTarget = true;
        }
        else if (strncmp(tagName, "up", 2) == 0)
        {
            this->up.x = childElement->FloatAttribute("x");
            this->up.y = childElement->FloatAttribute("y");
            this->up.z = childElement->FloatAttribute("z");
            
            seenUp = true;
        }
        else if (strncmp(tagName, "fov", 3) == 0)
        {
            this->fovy = childElement->FloatAttribute("value");

            seenFov = true;
        }
        else if (strncmp(tagName, "width", 5) == 0)
        {
            this->width = childElement->IntAttribute("value");

            seenWidth = true;
        }
        else if (strncmp(tagName, "height", 6) == 0)
        {
            this->height = childElement->IntAttribute("value");

            seenHeight = true;
        }
        else
        {
            LOG(ERROR) << "Unknown parameters for camera. " << tagName;
        }

        childElement = childElement->NextSiblingElement();
    }

    if (!seenPosition)
    {
        LOG(WARNING) << "Does not find the camera's position. Use default";
    }
    if (!seenTarget)
    {
        LOG(WARNING) << "Does not find the camera's target. Use default";
    }
    if (!seenUp)
    {
        LOG(WARNING) << "Does not find the camera's up direction. Use default";
    }
    if (!seenFov)
    {
        LOG(WARNING) << "Does not find the camera's fov. Use default";
    }
    if (!seenWidth || !seenHeight)
    {
        LOG(WARNING) << "Does not find the camera's resolution. Use default";
    }

    // Update the nearo, nearx and neary.
    // The world space is z-up right-hand.

    f32 tanFovY = tanf(this->fovy * 0.5f);
    f32 spany = this->target.Length() * tanFovY;
    f32 spanx = spany * (f32)this->width / (f32)this->height;

    this->nearo = this->target; 
    this->nearx = vec3(1.0f, 0.0f, 0.0f) * spanx; // FIXME: view.GetColumn(0).XYZ();
    this->nearz = vec3(0.0f, 0.0f, 1.0f) * spany; // FIXME: view.GetColumn(1).XYZ();

    return true;
}
    
Camera::RayIterator Camera::beginRay() 
{
    RayIterator rayIterator(this);
    return rayIterator;
}

Camera::RayIterator Camera::endRay() 
{
    RayIterator rayIterator(this);
    rayIterator._x = 0;
    rayIterator._y = this->height;
    return rayIterator;
}

CS6620_NAMESPACE_END
