/**
 * \file scene.cpp
 * \author lihw (lihw81@gmail.com)
 * \changelog
 * - 2019/09/01 initial check in
 *
 * The description of a ray tracing scene.
 */

#include "scene.hpp"

CS6620_NAMESPACE_BEGIN

SceneNode::SceneNode(const char *nameStr)
    : name(nameStr)
{
    this->type = Type::UNKNOWN;

    this->scaling = 1.0f;
    this->rotation.Zero();
    this->translation.Zero();

    this->transform.SetIdentity();
    this->globalTransform.SetIdentity();
}

SceneNode::~SceneNode()
{
}

GeometricNode::Geometric(const char *name) 
    : SceneNode(name)
{
    this->type = SceneNode::Type::GEOMETRIC;
}
    
void GeometricObject::_parseScale(XmlElement *xmlElement)
{
    assert(strncmp(xmlElement->GetText(), "scale", 5) == 0);
    this->scale = xmlElement->FloatAttribute("value");
}

void GeometricObject::_parseTranslate(XmlElement *xmlElement)
{
    assert(strncmp(xmlElement->GetText(), "translate", 9) == 0);
    this->translate.x = xmlElement->FloatAttribute("x");
    this->translate.y = xmlElement->FloatAttribute("y");
    this->translate.z = xmlElement->FloatAttribute("z");
}

void GeometricObject::_parseRotate(XmlElement *xmlElement)
{
    assert(strncmp(xmlElement->GetText(), "rotate", 6) == 0);
    this->rotate.x = xmlElement->FloatAttribute("x");
    this->rotate.y = xmlElement->FloatAttribute("y");
    this->rotate.z = xmlElement->FloatAttribute("z");
}

CS6620_NAMESPACE_END
