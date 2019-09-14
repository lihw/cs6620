/**
 * \file scene.cpp
 * \author lihw (lihw81@gmail.com)
 * \changelog
 * - 2019/09/01 initial check in
 *
 * The description of a ray tracing scene.
 */

#include "scene.hpp"

#include "scene_node.hpp"
#include "camera.hpp"
#include "tree.hpp"

#include <list>

CS6620_NAMESPACE_BEGIN

Scene::Scene()
{
}

Scene::~Scene()
{
    this->_destroy();
}

bool Scene::load(const char *sceneFile) noexcept
{
    // The scene has data already. Clear it first.
    if (this->root != nullptr)
    {
        LOG(WARNING) << "The scene's old data is cleared.";

        this->_destroy();
    }

    LOG(INFO) << "Start parsing XML '" << sceneFile << "'";
    
    tinyxml2::XMLDocument xmlDoc;
    if (xmlDoc.LoadFile(sceneFile) != tinyxml2::XML_SUCCESS)
    {
        LOG(ERROR) << "Fail to parse " << sceneFile;
        return false;
    }

    tinyxml2::XMLElement *rootElement = xmlDoc.FirstChildElement();
    if (strncmp(rootElement->Name(), "xml", 3) != 0)
    {
        LOG(ERROR) << "The scene's root node must be 'xml'";
        return false;
    }
    
    tinyxml2::XMLElement *sceneElement = rootElement->FirstChildElement();
    if (strncmp(sceneElement->Name(), "scene", 5) != 0)
    {
        LOG(ERROR) << "The scene node is not found under root node'";
        return false;
    }

    assert(this->root == nullptr);

    this->root = new SceneNode("root", nullptr);

    tinyxml2::XMLElement *nodeElement = sceneElement->FirstChildElement();
    while (nodeElement != nullptr)
    {
        if (strncmp(nodeElement->Name(), "object", 6) == 0)
        {
            SceneNode *node = SceneNodeFactory::unserialize(nodeElement, this->root);
            if (node == nullptr)
            {
                LOG(ERROR) << "Fail to unserialize " << nodeElement->Name();
                return false;
            }

            this->root->children.push_back(node);
        }
        
        nodeElement = nodeElement->NextSiblingElement();
    }

    tinyxml2::XMLElement *cameraElement = sceneElement->NextSiblingElement();
    if (cameraElement != nullptr && strncmp(cameraElement->Name(), "camera", 6) == 0)
    {
        assert(this->camera == nullptr);

        this->camera = new Camera();
        this->camera->unserialize(cameraElement);
    }
    
    LOG(INFO) << "Parsing XML '" << sceneFile << "' succeeds!";

    if (this->root == nullptr)
    {
        LOG(ERROR) << "'" << sceneFile << "' doesn't contain any nodes.";
        return false;
    }

    if (this->camera == nullptr)
    {
        LOG(WARNING) << "'" << sceneFile << "' doesn't contain any camera. Use default one!";

        this->camera = new Camera();
    }
 
    return true;
}

void Scene::prepare() noexcept
{
    this->_tree = new Tree(this);
}

void Scene::_destroy()
{
    // Delete the scene nodes using BFS.
    std::list<SceneNode *> nodes;
    nodes.push_back(this->root);
    while (!nodes.empty())
    {
        SceneNode *node = nodes.front();
        nodes.pop_front();

        nodes.insert(nodes.end(), node->children.begin(), node->children.end());

        delete node;
    }

    this->root = nullptr;
}

vec3 Scene::shade(const Ray &ray)
{
    vec3 result;

    SceneNode *node = nullptr;
    vec3 position;
    vec3 normal;
    if (this->_tree->intersect(ray, node, position, normal))
    {
        // Direct lighting.
        //result += this->_shader->shade(object, position, normal, scene->lights);
        result = vec3(0, 0, 0);

        // Indirect lighting.
        //Ray reflect;
        //this->_sampler->sample(object, position, ray, normal, &reflect);
        //if (this->_tree->intersect(ray, &object, position, normal))
        //{
        //    result 
        //}
    }
    else
    {
        result = vec3(1, 1, 1);
    }

    return result;
}


CS6620_NAMESPACE_END
