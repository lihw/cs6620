/**
 * \file scene_node.cpp
 * \author lihw (lihw81@gmail.com)
 * \changelog
 * - 2019/09/03 initial check in
 *
 * The descriptions of nodes in the ray tracing scene.
 */

#include "scene_node.hpp"

CS6620_NAMESPACE_BEGIN

SceneNode::SceneNode(const char *nameStr)
    : name(nameStr)
{
    this->type = Type::UNKNOWN;

    this->scale = 1.0f;
    this->rotate.Zero();
    this->translate.Zero();

    this->transform.Identity();
    this->globalTransform.Identity();
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
    assert(strncmp(xmlElement->GetName(), "translate", 9) == 0);
    this->translate.x = xmlElement->FloatAttribute("x");
    this->translate.y = xmlElement->FloatAttribute("y");
    this->translate.z = xmlElement->FloatAttribute("z");
}

void GeometricObject::_parseRotate(XmlElement *xmlElement)
{
    assert(strncmp(xmlElement->GetName(), "rotate", 6) == 0);
    this->rotate.x = xmlElement->FloatAttribute("x");
    this->rotate.y = xmlElement->FloatAttribute("y");
    this->rotate.z = xmlElement->FloatAttribute("z");
}

bool GeometricObject::unserialize(XmlElement *xmlElement) noexcept
{
    this->name = xmlElement->StringAttribute("name");
    this->type = xmlElement->StringAttribute("type");

    XmlElement *childElement = xmlElement->FirstChildElement();

    bool seenTranslate = false;
    bool seenScale = false;
    bool seenRotate  = false;

    // Parse the element and extract properties of the camera.
    while (childElement != nullptr)
    {
        const char *tagName = childElement->GetName();
        if (strncmp(tagName, "translate") == 0)
        {
            this->_parseTranslate(childElement);   
            seenTranslate = true;
        }
        else if (strncmp(tagName, "scale") == 0)
        {
            this->_parseScale(childElement);   
            seenScale = true;
        }
        else if (strncmp(tagName, "rotate") == 0)
        {
            this->_parseRotate(childElement);   
            seenRotate = true;
        }
        else if (strncmp(tagName, "object") == 0)
        {
            // See another node under this, we parse it recursively.
            GeometricNode *childNode = new GeometricNode();
            // This node is child of this one.
            childNode->parent = this;
            this->children.push_back(childNode);
            childNode->unserialize(childElement);
        }

        // Update the local transform.
        this->updateTransform(); 
        // Update the global transform.
        this->updateGlobalTransform();

        if (!seenTranslate || !seenRotate || !seenScale)
        {
            LOG(WARNING) << "Doesn't see all transform about Node ''" << this->name << "'. Use default values";
        }

        childElement = childElement->NextSiblingElement();
    }
}
    
void GeometricNode::_updateTransform()
{
    mat4 scaling;
    scaling.SetScale(this->scale);

    mat4 rotation;
    rotation.SetRotationXZY(this->rotate.x, this->rotate.y, this->rotate.z);

    mat4 translation;
    translation.SetTranslation(this->translate.x, this->translate.y, this->translate.z);
        
    this->transform = scaling * rotation * translation; // The matrix is row major.
}
    
void GeometricNode::_updateGlobalTransform()
{
    if (this->parent != nullptr)
    {
        this->globalTransform = this->transform * this->globalTransform;
    }
    else
    {
        this->globalTransform = this->transform;
    }
}

GeometricSphereNode::GeometricSphereNode()
    : GeometricNode()
{
}

GeometricSphereNode::~GeometricSphereNode()
{
}

bool GeometricSphereNode::unserialize(XmlElement *xmlElement) noexcept
{
    if (!GeometricNode::unserialize(xmlElement))
    {
        return false;
    }

    this->_position.x = this->globalTransform[12];
    this->_position.y = this->globalTransform[13];
    this->_position.z = this->globalTransform[14];

    this->_radius = this->globalTransform.GetRow(0).Length();
}

bool GeometricSphereNode::intersect(const Ray &ray) noexcept 
{
    // The projection of the distance between sphere and ray origin onto the ray direction.
    vec3 direction = this->_position - ray.origin;
    f32 projection = direction.Dot(ray.direction);

    // Compute the distance from the sphere center to the ray direction.
    f32 distance2 = direction.x * direction.x + direction.y * direction.y + direction.z * direction.z - 
        projection * projection 

    return distance2 <= this->_radius * this->_radius;
}

CS6620_NAMESPACE_END
