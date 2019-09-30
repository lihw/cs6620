/**
 * \file scene_node.cpp
 * \author lihw (lihw81@gmail.com)
 * \changelog
 * - 2019/09/03 initial check in
 *
 * The descriptions of nodes in the ray tracing scene.
 */

#include "scene_node.hpp"

#include "light.hpp"
#include "material.hpp"
#include "intersect.hpp"

#include <list>


CS6620_NAMESPACE_BEGIN

//
// class SceneNode
//
SceneNode::SceneNode(const char *nameStr, SceneNode *parent)
    : name(nameStr)
{
    this->category = Category::UNKNOWN;

    this->scale = vec3(1.0f, 1.0f, 1.0f);
    this->rotate.Zero();
    this->translate.Zero();

    this->transform = mat4::Identity();
    this->globalTransform = mat4::Identity();

    this->parent = parent;
}

SceneNode::~SceneNode()
{
}

bool SceneNode::unserialize(tinyxml2::XMLElement *xmlElement) noexcept
{
    this->name = xmlElement->Attribute("name");

    return true;
}

//
// class GeometricNode
//
GeometricNode::GeometricNode(const char *name, SceneNode *parent) 
    : SceneNode(name, parent)
{
    this->category = SceneNode::Category::GEOMETRY;
}

GeometricNode::~GeometricNode()
{
}
    
void GeometricNode::_parseScale(tinyxml2::XMLElement *xmlElement)
{
    f32 value;
    assert(strncmp(xmlElement->Name(), "scale", 5) == 0);
    if (xmlElement->QueryAttribute("value", &value) == tinyxml2::XML_SUCCESS)
    {
        this->scale = vec3(value, value, value);
    }
    else
    {
        f32 x;
        f32 y;
        f32 z;
        if (xmlElement->QueryAttribute("x", &x) == tinyxml2::XML_SUCCESS &&
            xmlElement->QueryAttribute("y", &y) == tinyxml2::XML_SUCCESS &&
            xmlElement->QueryAttribute("z", &z) == tinyxml2::XML_SUCCESS)
        {
            this->scale = vec3(x, y, z);
        }
    }
}

void GeometricNode::_parseTranslate(tinyxml2::XMLElement *xmlElement)
{
    assert(strncmp(xmlElement->Name(), "translate", 9) == 0);
    this->translate.x = xmlElement->FloatAttribute("x");
    this->translate.y = xmlElement->FloatAttribute("y");
    this->translate.z = xmlElement->FloatAttribute("z");
}

void GeometricNode::_parseRotate(tinyxml2::XMLElement *xmlElement)
{
    assert(strncmp(xmlElement->Name(), "rotate", 6) == 0);
    this->rotate.x = xmlElement->FloatAttribute("x");
    this->rotate.y = xmlElement->FloatAttribute("y");
    this->rotate.z = xmlElement->FloatAttribute("z");
}

bool GeometricNode::unserialize(tinyxml2::XMLElement *xmlElement) noexcept
{
    SceneNode::unserialize(xmlElement);

    const char *material = xmlElement->Attribute("material");
    if (material != nullptr)
    {
        this->materialName = material;
    }

    tinyxml2::XMLElement *childElement = xmlElement->FirstChildElement();

    bool seenTranslate = false;
    bool seenScale = false;
    bool seenRotate  = false;

    // Caching the child element node. We can process child elements 
    // before finishing their parent. Otherwise since parent's transform
    // is not yet setup, the global transform of child elements are not 
    // correct.
    std::list<tinyxml2::XMLElement *> childXmlElements;

    // Parse the element and extract properties of the camera.
    while (childElement != nullptr)
    {
        const char *tagName = childElement->Name();
        if (strncmp(tagName, "translate", 9) == 0)
        {
            this->_parseTranslate(childElement);   
            seenTranslate = true;
        }
        else if (strncmp(tagName, "scale", 5) == 0)
        {
            this->_parseScale(childElement);   
            seenScale = true;
        }
        else if (strncmp(tagName, "rotate", 6) == 0)
        {
            this->_parseRotate(childElement);   
            seenRotate = true;
        }
        else if (strncmp(tagName, "object", 6) == 0)
        {
            childXmlElements.push_back(childElement);
        }

        childElement = childElement->NextSiblingElement();
    }

    // Update the local transform.
    this->_updateTransform();
    // Update the global transform.
    this->_updateGlobalTransform();

    if (!seenTranslate || !seenRotate || !seenScale)
    {
        LOG(WARNING) << "Doesn't see all transform about Node ''" << this->name << "'. Use default values";
    }

    for (auto &&childXmlElement : childXmlElements)
    {
        SceneNode *childNode = SceneNodeFactory::unserialize(childXmlElement, this);
        if (childNode != nullptr)
        {
            this->children.push_back(childNode);
        }
        else
        {
            LOG(ERROR) << "Fail to unserialize " << childElement->Name();
            // FIXME: in a recursion call, return from a call at the bottom doesn't
            // quit the entire call stack.
            return false;
        }
    }

    return true;
}
    
void GeometricNode::_updateTransform()
{
    mat4 scaling;
    scaling.SetScale(this->scale);

    mat4 rotation;
    rotation.SetRotationXYZ(this->rotate.x, this->rotate.y, this->rotate.z);

    mat4 translation;
    translation.SetTranslation(vec3(this->translate.x, this->translate.y, this->translate.z));
        
    this->transform = translation * rotation * scaling; 
}
    
void GeometricNode::_updateGlobalTransform()
{
    if (this->parent != nullptr)
    {
        this->globalTransform = this->parent->globalTransform * this->transform;
    }
    else
    {
        this->globalTransform = this->transform;
    }
}

GeometricSphereNode::GeometricSphereNode(const char *name, SceneNode *parent)
    : GeometricNode(name, parent)
{
}

GeometricSphereNode::~GeometricSphereNode()
{
}

bool GeometricSphereNode::unserialize(tinyxml2::XMLElement *xmlElement) noexcept
{
    if (!GeometricNode::unserialize(xmlElement))
    {
        return false;
    }

    this->_position.x = this->globalTransform[12];
    this->_position.y = this->globalTransform[13];
    this->_position.z = this->globalTransform[14];

    this->_radius = this->globalTransform.GetRow(0).Length();

    return true;
}

bool GeometricSphereNode::intersect(const Ray &ray, vec3 &out_position, vec3 &out_normal) noexcept
{
    if (this->scale.x == this->scale.y == this->scale.z)
    {
        return Intersect::ray_sphere(ray, this->_position, this->_radius, 
                &out_position, &out_normal);
    }
    else
    {
        const Ray rayt = ray.getTransformed(this->globalTransform.GetInverse());

        return Intersect::ray_sphere(rayt, vec3(0, 0, 0), 1.0f, 
                &out_position, &out_normal);

        out_position = (this->globalTransform * out_position).XYZ();
        out_normal = (this->globalTransform.GetInverse().GetTranspose() * out_normal).XYZ();
    }
}

    
//
// class SceneNodeFactory
//
SceneNodeFactory::SceneNodeFactory()
{
}

SceneNodeFactory::~SceneNodeFactory()
{
}

SceneNode *SceneNodeFactory::unserialize(tinyxml2::XMLElement *xmlElement, SceneNode *parent) noexcept
{
    const char *type = xmlElement->Attribute("type");
    const char *name = xmlElement->Attribute("name");

    if (strncmp(type, "sphere", 6) == 0)
    {
        GeometricSphereNode *node = new GeometricSphereNode(name, parent);
        if (node->unserialize(xmlElement))
        {
            return node;
        }
    }

    return nullptr;
}

CS6620_NAMESPACE_END
