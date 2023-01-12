#include "scene_manager.hpp"
#include "prim_exception.hpp"
#include <fstream>
#include <sstream>
#include <unordered_map>
#include <stack>
#include "./nodes/node.hpp"
#include "utils.hpp"
#include "globals.hpp"
#include "resource_manager.hpp"

namespace prim
{
    Node* SceneManager::loadScene(std::string resPath)
    {
        fs::path path(ResourceManager::createResourcePath(resPath, true));

        std::ifstream stream(path.string(), std::ios::in);
        if (!stream.good()) throw PRIM_EXCEPTION("Unable to open file stream.");

        std::string line;

        Node* node = nullptr;
        std::stack<Node*> parentNodes;
        Node rootNode("Root");
        parentNodes.push(&rootNode);
        std::unordered_map<std::string, std::string> fields;

        while (true)
        {
            std::getline(stream, line);
            if (stream.eof()) break;

            if (line.empty()) continue;

            if (line == SceneFileSymbols::childrenStart)
            {
                node = NodeFactory::createNode(fields[SceneFileSymbols::type]);
                node->deserialize(fields);
                if (!parentNodes.empty()) parentNodes.top()->addChild(node);
                parentNodes.push(node);
                fields.clear();
                continue;
            }

            if (line == SceneFileSymbols::childrenEnd)
            {
                if (!fields.empty())
                {
                    node = NodeFactory::createNode(fields[SceneFileSymbols::type]);
                    node->deserialize(fields);
                    parentNodes.top()->addChild(node);
                }
                parentNodes.pop();
                node = nullptr;
                continue;
            }

            std::pair<std::string, std::string> keyValuePair = Utils::parseKeyValuePair(line);
            fields.insert(keyValuePair);
        }

        if (!fields.empty())
        {
            node = NodeFactory::createNode(fields[SceneFileSymbols::type]);
            node->deserialize(fields);
            parentNodes.top()->addChild(node);
            fields.clear();
            node = nullptr;
        }

        stream.close();

        if(parentNodes.top()->getChildren().size() > 1) throw PRIM_EXCEPTION("Scene file is corrupted or was written badly. More than one root nodes");
        rootNode.removeChild(parentNodes.top()->getChildren().front());
        return parentNodes.top()->getChildren().front();
    }

    void SceneManager::saveScene(Node* scene, std::string resPath, bool ovewrite)
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

    void SceneManager::freeScene(Node* scene)
    {
        Globals::logger->logInfo("Scene freed: '" + scene->getName() + "'");
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