#include "scene_manager.hpp"
#include "prim_exception.hpp"
#include <fstream>
#include <sstream>
#include <unordered_map>
#include <stack>
#include "nodes/node.hpp"
#include "utils.hpp"
#include "globals.hpp"
#include "resource_manager.hpp"

namespace prim
{
    Node* SceneManager::loadScene(std::string resPath) const
    {
        fs::path path(ResourceManager::createResourcePath(resPath, true));

        std::ifstream stream(path.string(), std::ios::in);
        if (!stream.good()) throw PRIM_EXCEPTION("Failed to load scene: Unable to open file stream.");

        std::stringstream fileSs;
        fileSs << stream.rdbuf();

        return Node::deserialize(fileSs.str());
    }

    void SceneManager::saveScene(Node* scene, std::string resPath, bool ovewrite) const
    {
        fs::path path(ResourceManager::createResourcePath(resPath, ovewrite));

        if(fs::exists(path) && !ovewrite)
            throw PRIM_EXCEPTION("Scene already exists, can't save. Path: " + resPath);

        if(!path.has_extension())
            path.replace_extension(sceneFileExtension);
        
        std::ofstream stream(path.string(), std::ios::out | std::ios::trunc);
        if (!stream.good()) throw PRIM_EXCEPTION("Unable to open file stream.");

        scene->setName(path.stem().string());

        stream << scene->serialize();

        stream.close();
    }

    void SceneManager::freeScene(Node* scene) const
    {
        Logger::inst().logInfo("Scene freed: '" + scene->getName() + "'");
        for (Node* child : scene->getChildren())
            freeScene(child);
        delete scene;
        Globals::sceneEditor->setSelectedNode(nullptr);
    }
    
    Node* SceneManager::createEmptyScene() const
    {
        return new Node("Empty Scene");
    }
}