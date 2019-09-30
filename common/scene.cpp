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
#include "light.hpp"
#include "material.hpp"

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
        else if (strncmp(nodeElement->Name(), "light", 5) == 0)
        {
            Light *light = new Light(nodeElement->Attribute("name"), this->root);
            if (!light->unserialize(nodeElement))
            {
                LOG(ERROR) << "Fail to unserialize " << nodeElement->Name();
                return false;
            }

            this->_lights.push_back(light);
        }
        else if (strncmp(nodeElement->Name(), "material", 8) == 0)
        {
            Material *material = MaterialFactory::unserialize(nodeElement);
            if (material == nullptr)
            {
                LOG(ERROR) << "Fail to unserialize " << nodeElement->Name();
                return false;
            }

            this->_materials.push_back(material);
        }
        
        nodeElement = nodeElement->NextSiblingElement();
    }

    tinyxml2::XMLElement *cameraElement = sceneElement->NextSiblingElement();
    if (cameraElement != nullptr && strncmp(cameraElement->Name(), "camera", 6) == 0)
    {
        assert(this->camera == nullptr);

        this->camera = new Camera();
        if (!this->camera->unserialize(cameraElement))
        {
            LOG(ERROR) << "Fail to unserialize camera '" << cameraElement->Name() << "'";
            return false;
        }
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

    /**
     * Find the material object for each scene node.
     */
    std::list<SceneNode *> nodes;
    nodes.push_back(this->root);
    while (!nodes.empty())
    {
        SceneNode *node = nodes.front();
        nodes.pop_front();

        nodes.insert(nodes.end(), node->children.begin(), node->children.end());

        if (node->materialName != "")
        {
            for (auto &&material : this->_materials)
            {
                if (material->name == node->materialName)
                {
                    node->material = material;
                    break;
                }
            }
        }
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

    /**
     * Destroy lights and materials of the scene.
     */
    for (auto &&light : this->_lights)
    {
        delete light;
    }
    for (auto &&material : this->_materials)
    {
        delete material;
    }
}

vec3 Scene::shade(const Ray &ray)
{
    vec3 result;

    SceneNode *node = nullptr;
    vec3 position;
    vec3 normal;
    if (this->_tree->intersect(ray, node, position, normal))
    {
        result = vec3(0, 0, 0);
        if (node->material != nullptr)
        {
            /**
             * Accumulate the direct lighting by each light in the scene 
             * to this surfels.
             */
            const vec3 view = (position - this->camera->position).GetNormalized();
            for (auto &light : this->_lights)
            {
                result += node->material->shade(*light, light->incident(position), view, normal);
            }
        }

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
