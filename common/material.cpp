/**
 * \file material.cpp
 * \author lihw (lihw81@gmail.com)
 * \changelog
 * - 2019/09/15 initial check in
 *
 * The descriptions of lights.
 */

#include "material.hpp"


CS6620_NAMESPACE_BEGIN

Material::Material(const char *name, SceneNode *parent)
{
}

Material::~Material()
{
}

BlinnMaterial::BlinnMaterial(const char *name, SceneNode *parent)
    : Material(name, parent)
{
}

BlinnMaterial::~BlinnMaterial()
{
}
    
bool BlinnMaterial::unserialize(tinyxml2::XMLElement *xmlElement) noexcept 
{
    SceneNode::unserialize(xmlElement);

    tinyxml2::XMLElement *childElement = xmlElement->FirstChildElement();

    bool seenSpecular   = false;
    bool seenDiffuse    = false;
    bool seenGlossiness = false;

    // Parse the element and extract properties of the blinn material.
    while (childElement != nullptr)
    {
        const char *tagName = childElement->Name();
        if (strncmp(tagName, "diffuse", 9) == 0)
        {
            this->_parseDiffuse(childElement);   
            seenDiffuse = true;
        }
        else if (strncmp(tagName, "specular", 5) == 0)
        {
            this->_parseSpecular(childElement);   
            seenSpecular = true;
        }
        else if (strncmp(tagName, "glossiness", 6) == 0)
        {
            this->_parseGlossiness(childElement);   
            seenGlossiness = true;
        }

        childElement = childElement->NextSiblingElement();
    }

    if (!seenDiffuse || !seenSpecular || !seenGlossiness)
    {
        LOG(WARNING) << "Doesn't see all properties about blinn material ''" << this->name << "'. Use default values";
    }

    return true;
}
    
vec3 BlinnMaterial::shade(const vec3 &incident, vec3 &reflection, vec3 &normal) const noexcept
{
}

CS6620_NAMESPACE_END
