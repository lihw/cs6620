/**
 * \file material.cpp
 * \author lihw (lihw81@gmail.com)
 * \changelog
 * - 2019/09/15 initial check in
 *
 * The descriptions of lights.
 */

#include "material.hpp"

#include "scene_node.hpp"
#include "light.hpp"

CS6620_NAMESPACE_BEGIN

Material::Material(const char *name1, const char *type1)
    : name(name1)
    , type(type1)
{

}

Material::~Material()
{
}
    
bool Material::unserialize(tinyxml2::XMLElement *xmlElement) noexcept
{
    return true;
}

vec3 Material::shade(const Light &light, const vec3 &incident, const vec3 &reflection, 
        const vec3 &normal) const noexcept
{
    return vec3(0, 0, 0);
}

BlinnMaterial::BlinnMaterial(const char *name)
    : Material(name, "blinn")
{
}

BlinnMaterial::~BlinnMaterial()
{
}
    
bool BlinnMaterial::unserialize(tinyxml2::XMLElement *xmlElement) noexcept 
{
    Material::unserialize(xmlElement);

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
    
vec3 BlinnMaterial::shade(const Light &light, const vec3& incident, const vec3 &reflected, const vec3 &normal) const noexcept
{
    /**
     * Blinn equation: 
     * ambient + diffuse * dot(n, l) + specular * (cos(dot(n, h)))^glossiness
     */
    switch (light.type)
    {
        case Light::AMBIENT:
            return vec3(light.intensity, light.intensity, light.intensity);
        case Light::DIRECT:
        case Light::POINT:
            {
                f32 ndotl = -incident.Dot(normal);
                vec3 reflect = normal * -2.0f * incident.Dot(normal) + incident;
                vec3 h = (reflect + normal) * 0.5f;
                f32 ndoth = h.Dot(normal.GetNormalized());
                return ndotl * this->diffuse + pow(ndoth, this->glossiness) * this->specular;
            }
        default:
            LOG(ERROR) << "Unsupport light type!";
            break;
    }

    return vec3(0.0f, 0.0f, 0.0f);
}

void BlinnMaterial::_parseGlossiness(tinyxml2::XMLElement *xmlElement)
{
    assert(strncmp(xmlElement->Name(), "glossiness", 10) == 0);

    this->glossiness = xmlElement->FloatAttribute("value");
}

void BlinnMaterial::_parseDiffuse(tinyxml2::XMLElement *xmlElement)
{
    assert(strncmp(xmlElement->Name(), "diffuse", 7) == 0);

    f32 r = xmlElement->FloatAttribute("r");
    f32 g = xmlElement->FloatAttribute("g");
    f32 b = xmlElement->FloatAttribute("b");
    
    f32 value = 1.0f;
    if (xmlElement->QueryFloatAttribute("value", &value) == tinyxml2::XML_SUCCESS)
    {
        r *= value;
        g *= value;
        b *= value;
    }
    this->diffuse.x = r;
    this->diffuse.y = g;
    this->diffuse.z = b;
}

void BlinnMaterial::_parseSpecular(tinyxml2::XMLElement *xmlElement)
{
    assert(strncmp(xmlElement->Name(), "specular", 8) == 0);

    f32 r = xmlElement->FloatAttribute("r");
    f32 g = xmlElement->FloatAttribute("g");
    f32 b = xmlElement->FloatAttribute("b");
    
    f32 value = 1.0f;
    if (xmlElement->QueryFloatAttribute("value", &value) == tinyxml2::XML_SUCCESS)
    {
        r *= value;
        g *= value;
        b *= value;
    }
    this->specular.x = r;
    this->specular.y = g;
    this->specular.z = b;
}
    
Material *MaterialFactory::unserialize(tinyxml2::XMLElement *xmlElement)
{
    const char *type = xmlElement->Attribute("type");
    const char *name = xmlElement->Attribute("name");

    if (strncmp(type, "blinn", 5) == 0)
    {
        BlinnMaterial *node = new BlinnMaterial(name);
        if (node->unserialize(xmlElement))
        {
            return node;
        }
    }

    return nullptr;
}

CS6620_NAMESPACE_END
