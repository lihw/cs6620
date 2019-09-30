/**
 * \file light.cpp
 * \author lihw (lihw81@gmail.com)
 * \changelog
 * - 2019/09/15 initial check in
 *
 * The descriptions of lights.
 */

#include "light.hpp"

#include "scene.hpp"

CS6620_NAMESPACE_BEGIN

Light::Light(const char *name, SceneNode *parent)
    : SceneNode(name, parent)
{
    type = AMBIENT;
    this->intensity = 1.0;
    this->position = vec3(1.0f, 0.0f, 0.0f);
}

Light::~Light()
{
}

bool Light::unserialize(tinyxml2::XMLElement *xmlElement) noexcept 
{
    SceneNode::unserialize(xmlElement);

    const char *type = xmlElement->Attribute("type");
    if (strncmp(type, "ambient", 5) == 0)
    {
        this->type = AMBIENT;
    }
    else if (strncmp(type, "direct", 5) == 0)
    {
        this->type = DIRECT;
    }
    else if (strncmp(type, "point", 5) == 0)
    {
        this->type = POINT;
    }
    else
    {
        LOG(ERROR) << "Unsupport light type!";
    }

    tinyxml2::XMLElement *childElement = xmlElement->FirstChildElement();

    bool seenIntensity = false;
    bool seenDirection = false;
    bool seenPosition = false;
    // Parse the element and extract properties of the light.
    while (childElement != nullptr)
    {
        const char *tagName = childElement->Name();
        if (strncmp(tagName, "intensity", 9) == 0)
        {
            this->_parseIntensity(childElement);   
            seenIntensity = true;
        }
        else if (strncmp(tagName, "direction", 5) == 0)
        {
            this->_parseDirection(childElement);   
            seenDirection = true;
        }
        else if (strncmp(tagName, "position", 6) == 0)
        {
            this->_parsePosition(childElement);   
            seenPosition = true;
        }

        childElement = childElement->NextSiblingElement();
    }

    if (!seenIntensity ||
        (this->type == DIRECT && !seenDirection) ||
        (this->type == POINT && !seenPosition))
    {
        LOG(WARNING) << "Doesn't see all properties about light ''" << this->name << "'. Use default values";
    }

    return true;
}

void Light::_parseIntensity(tinyxml2::XMLElement *xmlElement)
{
    assert(strncmp(xmlElement->Name(), "intensity", 9) == 0);
    this->intensity = xmlElement->FloatAttribute("value");
}
    
void Light::_parseDirection(tinyxml2::XMLElement *xmlElement)
{
    assert(strncmp(xmlElement->Name(), "direction", 9) == 0);
    this->direction.x = xmlElement->FloatAttribute("x");
    this->direction.y = xmlElement->FloatAttribute("y");
    this->direction.z = xmlElement->FloatAttribute("z");
}

void Light::_parsePosition(tinyxml2::XMLElement *xmlElement)
{
    assert(strncmp(xmlElement->Name(), "position", 8) == 0);
    this->position.x = xmlElement->FloatAttribute("x");
    this->position.y = xmlElement->FloatAttribute("y");
    this->position.z = xmlElement->FloatAttribute("z");
}
    
vec3 Light::incident(const vec3 &position) const
{
    switch (this->type)
    {
        case AMBIENT:
            return this->direction;
        case DIRECT:
            return this->direction;
        case POINT:
            return (position - this->position).GetNormalized();
        default:
            LOG(ERROR) << "Unsupport light type!";
            break;
    }

    return vec3(0, 0, 0);
}

CS6620_NAMESPACE_END


