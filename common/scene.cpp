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

Scene::Scene()
{
}

Scene::~Scene()
{
    this->_destroy();
}

bool load(const char *sceneFile) noexcept
{
    // The scene has data already. Clear it first.
    if (this->_root != nullptr)
    {
        LOG(WARNING) << "The scene's old data is cleared.";

        this->_destroy();
    }

    LOG(INFO) << "Start parsing XML '" << sceneFile << "'";
    
    XMLDocument xmlDoc;
    if (!xmlDoc.LoadFile(sceneFile))
    {
        LOG(ERROR) << "Fail to parse " << sceneFile;
        return false;
    }

    XmlElement *rootElement = xmlDoc.FirstChildElement();
    if (strncmp(rootElement->GetName(), "xml") == 0)
    {
        LOG(ERROR) << "The scene's root node must be 'xml'";
        return false;
    }
    
    XmlElement *sceneElement = rootElement->FirstChildElement();
    if (strncmp(sceneElement->GetName(), "scene") == 0)
    {
        LOG(ERROR) << "The scene node is not found under root node'";
        return false;
    }

    XmlElement *nodeElement = sceneElement->FirstChildElement();
    while (nodeElement != nullptr)
    {
        if (strncmp(nodeElement->GetName(), "object") == 0)
        {
            // TODO: use factory pattern!
            GeometricNode *node = new GeometricNode();
            node->nserialize(nodeElement);

            this->_nodes.push_back(node);

            if (this->_root == nullptr)
            {
                this->_root = node;
            }
        }
        else if (strncmp(nodeElement->GetName(), "camera") == 0)
        {
            if (this->_camera == nullptr)
            {
                this->_camera = new Camera();
            }
            this->_camera->unserialize(nodeElement);
        }
        nodeElement = nodeElement->NextSiblingElement();
    }
    
    LOG(INFO) << "Parsing XML '" << sceneFile << "' succeeds!";

    if (this->_nodes.empty())
    {
        LOG(ERROR) << "'" << sceneFile << "' doesn't contain any nodes.";
        return false;
    }

    if (this->_camera == nullptr)
    {
        LOG(WARNING) << "'" << sceneFile << "' doesn't contain any camera. Use default one!";

        this->_camera = new Camera();
    }
 
    return true;
}

void Scene::_destroy()
{
    for (auto &&node : this->_nodes)
    {
        delete node;
    }

    if (this->_camera)
    {
        delete this->_camera;
    }
}


CS6620_NAMESPACE_END
