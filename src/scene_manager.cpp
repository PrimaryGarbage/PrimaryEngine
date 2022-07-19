#include "scene_manager.hpp"
#include "prim_exception.hpp"
#include <fstream>
#include <filesystem>
#include <sstream>
#include "node.hpp"

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


    void SceneManager::loadScene(std::string fileName, Node* parentNode)
    {
        fs::path path(savePath + fileName + sceneFileExtension);
        if (!fs::exists(path)) throw PRIM_EXCEPTION("File with the path '" + path.string() + "' doesn't exists.");
        
        std::ifstream stream(path.string(), std::ios::in);
        if (!stream.good()) throw PRIM_EXCEPTION("Unable to open file stream.");

        std::stringstream ss;
        ss << stream.rdbuf();
        std::string data = std::move(ss.str());


        stream.close();
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

}