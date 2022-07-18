#include "scene_manager.hpp"
#include "prim_exception.hpp"
#include <fstream>
#include <filesystem>


namespace prim
{
    std::string SceneManager::serialize(Node* node)
    {
        
    }

    namespace fs = std::filesystem;

    void SceneManager::loadScene(const char* filePath, Node* parentNode)
    {
    }
    
    void SceneManager::saveScene(Node* scene, const char* filePath, bool ovewrite)
    {
        fs::path path(filePath);
        if(fs::exists(path) && !ovewrite) throw PRIM_EXCEPTION("File with the path '" + std::string(filePath) + "' already exists.");

        std::ofstream stream(path.string(), std::ios::out | std::ios::trunc);
        if(!stream.good()) throw PRIM_EXCEPTION("Unable to open file stream.");

        

        /// seialize scene and write it to the file //
    }

}