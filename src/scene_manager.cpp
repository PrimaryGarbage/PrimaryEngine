#include "scene_manager.hpp"
#include "prim_exception.hpp"
#include <fstream>
#include <filesystem>
#include "node.hpp"

namespace prim
{
    namespace fs = std::filesystem;
   
    SceneManager::SceneManager(const char* savePath) : savePath(savePath)
    {
        fs::path path(savePath);
        if(!fs::exists(path))
        {
            fs::create_directories(path);
        }
    }


    void SceneManager::loadScene(std::string fileName, Node* parentNode)
    {
    }
    
    void SceneManager::saveScene(Node* scene, std::string fileName, bool ovewrite)
    {
        fs::path path(savePath + fileName + sceneFileExtension);
        if(fs::exists(path) && !ovewrite) throw PRIM_EXCEPTION("File with the path '" + path.string() + "' already exists.");

        std::ofstream stream(path.string(), std::ios::out | std::ios::trunc);
        if(!stream.good()) throw PRIM_EXCEPTION("Unable to open file stream.");

        stream << scene->serialize();

        stream.close();
    }

}