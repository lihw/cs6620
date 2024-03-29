/**
 * \file scene_node.cpp
 * \author lihw (lihw81@gmail.com)
 * \changelog
 * - 2019/09/03 initial check in
 *
 * The descriptions of nodes in the ray tracing scene.
 */

#include "scene_node.hpp"

#include <list>

CS6620_NAMESPACE_BEGIN

//
// class SceneNode
//
SceneNode::SceneNode(const char *nameStr, SceneNode *parent)
    : name(nameStr)
{
    this->type = Type::UNKNOWN;

    this->scale = 1.0f;
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
    this->type = SceneNode::Type::GEOMETRY;
}

GeometricNode::~GeometricNode()
{
}
    
void GeometricNode::_parseScale(tinyxml2::XMLElement *xmlElement)
{
    assert(strncmp(xmlElement->Name(), "scale", 5) == 0);
    this->scale = xmlElement->FloatAttribute("value");
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
    // The projection of the distance between sphere and ray origin onto the ray direction.
    vec3 direction = this->_position - ray.origin;
    f32 projection = direction.Dot(ray.direction);

    // Compute the distance from the sphere center to the ray direction.
    f32 distance2 = direction.LengthSquared() - projection * projection;

    return distance2 <= this->_radius * this->_radius;
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
