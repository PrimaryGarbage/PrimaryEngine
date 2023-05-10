#include "scene_manager.hpp"
#include "prim_exception.hpp"
#include <fstream>
#include <sstream>
#include <algorithm>
#include "nodes/node.hpp"
#include "utils.hpp"
#include "globals.hpp"
#include "resource_manager.hpp"

namespace prim
{
    std::vector<Node*> SceneManager::loadScene(std::string resPath) const
    {
        fs::path path(ResourceManager::createResourcePath(resPath, true));

        std::ifstream stream(path.string(), std::ios::in);
        if (!stream.good()) throw PRIM_EXCEPTION("Failed to load scene: Unable to open file stream.");

        std::stringstream fileSs;
        fileSs << stream.rdbuf();

        Node* root = Node::deserialize(fileSs.str());

        // std::vector<Node*> nodes(root->children);
        // std::for_each(nodes.begin(), nodes.end(), [](Node* node) { node->orphanize(); });
        // delete root;

        return std::vector<Node*>{root};
    }

    void SceneManager::saveScene(const std::vector<Node*>& scene, std::string resPath, bool ovewrite) const
    {
        fs::path path(ResourceManager::createResourcePath(resPath, ovewrite));

        if(fs::exists(path) && !ovewrite)
            throw PRIM_EXCEPTION("Scene already exists, can't save. Path: " + resPath);

        if(!path.has_extension())
            path.replace_extension(sceneFileExtension);
        
        std::ofstream stream(path.string(), std::ios::out | std::ios::trunc);
        if (!stream.good()) throw PRIM_EXCEPTION("Unable to open file stream.");

        Node* root = Node::createNode("Node");
        root->addChildren(scene);

        root->setName(path.stem().string());

        stream << root->serialize();

        std::for_each(scene.begin(), scene.end(), [](Node* node) { node->orphanize(); });
        delete root;
        stream.close();
    }

    void SceneManager::freeScene(std::vector<Node*>& scene) const
    {
        for (Node* node : scene)
            delete node;
        Logger::inst().logInfo("Scene freed.");
        Globals::sceneEditor->setSelectedNode(nullptr);
    }
}