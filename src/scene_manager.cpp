#include "scene_manager.hpp"
#include "prim_exception.hpp"
#include <fstream>
#include <filesystem>
#include <sstream>
#include <unordered_map>
#include <stack>
#include "node_utils.hpp"

namespace prim
{
    namespace fs = std::filesystem;

    SceneManager::SceneManager(const char* savePath) : savePath(savePath)
    {
        fs::path path(savePath);
        if (!fs::exists(path))
        {
            fs::create_directories(path);
        }
    }


    Node* SceneManager::loadScene(std::string fileName)
    {
        fs::path path(savePath + fileName + sceneFileExtension);
        if (!fs::exists(path)) throw PRIM_EXCEPTION("File with the path '" + path.string() + "' doesn't exists.");

        std::ifstream stream(path.string(), std::ios::in);
        if (!stream.good()) throw PRIM_EXCEPTION("Unable to open file stream.");

        std::string line;

        Node* node = nullptr;
        std::stack<std::pair<Node*, std::unordered_map<std::string, std::string>>> parentNodes;
        std::unordered_map<std::string, std::string> fields;

        while (true)
        {
            std::getline(stream, line);
            if (stream.eof()) break;

            if (line.empty()) continue;

            if (line == NodeFields::childrenStart)
            {
                node = createNode(fields[NodeFields::type].c_str(), fields);
                if (!parentNodes.empty()) parentNodes.top().first->addChild(node);
                parentNodes.push(std::pair(node, fields));
                fields.clear();
                continue;
            }

            if (line == NodeFields::childrenEnd)
            {
                if (!fields.empty())
                {
                    node = createNode(fields[NodeFields::type].c_str(), fields);
                    parentNodes.top().first->addChild(node);
                }

                parentNodes.pop();
                node = nullptr;
                continue;
            }

            std::pair<std::string, std::string> keyValuePair = parseKeyValuePair(line);
            fields.insert(keyValuePair);
        }

        if (!fields.empty())
        {
            node = createNode(fields[NodeFields::type].c_str(), fields);
            parentNodes.top().first->addChild(node);
            fields.clear();
            node = nullptr;
        }

        stream.close();

        return parentNodes.top().first;
    }

    void SceneManager::saveScene(Node* scene, std::string fileName, bool ovewrite)
    {
        fs::path path(savePath + fileName + sceneFileExtension);
        if (fs::exists(path) && !ovewrite) throw PRIM_EXCEPTION("File with the path '" + path.string() + "' already exists.");

        std::ofstream stream(path.string(), std::ios::out | std::ios::trunc);
        if (!stream.good()) throw PRIM_EXCEPTION("Unable to open file stream.");

        stream << scene->serialize();

        stream.close();
    }

    void SceneManager::freeScene(Node* scene)
    {
        for (Node* child : scene->getChildren())
            freeScene(child);
        delete scene;
    }
}