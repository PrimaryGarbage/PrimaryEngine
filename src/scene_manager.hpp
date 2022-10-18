#ifndef __SCENE_MANAGER_HPP__
#define __SCENE_MANAGER_HPP__

#include <string>
#include <filesystem>
#include "typedefs.hpp"

namespace prim
{
    class Node;

    class SceneManager
    {
    private:

        struct SceneFileSymbols
        {
            inline static const char* header = "Node";
            inline static const char* type = "type";
            inline static const char* childrenStart = "children_start";
            inline static const char* childrenEnd = "children_end";
        };

        const static inline char* sceneFileExtension = ".psc";
        const static inline char keyValueSeparator = '=';
        const static inline char* sceneDirectory = "res/scenes/";
        fs::path savePath;

        std::string serialize(Node* node);
        std::filesystem::path createPathToScene(std::string sceneName);
    public:
        SceneManager(fs::path appDirPath);

        Node* loadScene(std::string name);
        void saveScene(Node* scene, std::string name, bool ovewrite = false);
        void freeScene(Node* scene);
        bool sceneExists(std::string name);
        const fs::path& getSceneDirPath() const;
    };
}

#endif // __SCENE_MANAGER_HPP__