/**
 * \file camera.cpp
 * \author lihw (lihw81@gmail.com)
 * \changelog
 * - 2019/09/03 initial check in
 *
 * The description of the camera in a ray tracing scene.
 */

#include "camera.hpp"

CS6620_NAMESPACE_BEGIN

Camera::Camera()
{
    this->position = vec3f(0.0f, 0.0f, 0.0f);
    this->target   = vec3(0.0f, 0.0f, 0.0f);
    this->up       = vec3(0.0f, 1.0f, 0.0f);
    this->fov      = 45.0f;
    this->width    = 512;
    this->height   = 512;
}

Camera::~Camera()
{
}

bool Camera::unserialize(XmlElement *xmlElement) noexcept 
{
    XmlElement *childElement = xmlElement->FirstChildElement();

    bool seenPosition = false;
    bool seenTarget = false;
    bool seenFov = false;
    bool seenWidth = false;
    bool seenHeight = false;

    // Parse the element and extract properties of the camera.
    while (childElement != nullptr)
    {
        const char *tagName = childElement->GetName();
        if (strncmp(tagName, "position") == 0)
        {
            this->position.x = xmlElement->FloatAttribute("x");
            this->position.y = xmlElement->FloatAttribute("y");
            this->position.z = xmlElement->FloatAttribute("z");

            seenPosition = true;
        }
        else if (strncmp(tagName, "target") == 0)
        {
            this->target.x = xmlElement->FloatAttribute("x");
            this->target.y = xmlElement->FloatAttribute("y");
            this->target.z = xmlElement->FloatAttribute("z");
            
            seenTarget = true;
        }
        else if (strncmp(tagName, "up") == 0)
        {
            this->up.x = xmlElement->FloatAttribute("x");
            this->up.y = xmlElement->FloatAttribute("y");
            this->up.z = xmlElement->FloatAttribute("z");
            
            seenUp = true;
        }
        else if (strncmp(tagName, "fov") == 0)
        {
            this->fov = xmlElement->FloatAttribute("value");

            seenFov = true;
        }
        else if (strncmp(tagName, "width") == 0)
        {
            this->width = xmlElement->IntAttribute("value");

            seenWidth = true;
        }
        else if (strncmp(tagName, "height") == 0)
        {
            this->height = xmlElement->IntAttribute("value");

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
    vec3 lookTo = target - position;
    lookTo.Normalize();

    mat4 view = View(this->position, this->target, this->up);

    this->nearo = this->position - view.GetColumn(2) * 1.0f; // The near plane is 1 meter away.
    this->nearx = view->GetColumn(0);
    this->neary = view->GetColumn(1);

    return true;
}
    
RayIterator Camera::beginRay() const
{
    RayIterator rayIterator(this);
    return rayIterator;
}

RayIterator Camera::endRay() const
{
    RayIterator rayIterator(this);
    rayIterator._x = 0;
    rayIterator._y = this->height;
    return rayIterator;
}

CS6620_NAMESPACE_END
