#ifndef __SCENE_MANAGER_HPP__
#define __SCENE_MANAGER_HPP__

#include <string>
#include <filesystem>

namespace prim
{
    class Node;

    class SceneManager
    {
    private:
        const static inline char* sceneFileExtension = ".psc";
        const static inline char keyValueSeparator = '=';
        const char* savePath;

        std::string serialize(Node* node);
        std::filesystem::path createPathToScene(std::string sceneName);
    public:
        SceneManager(const char* savePath);

        Node* loadScene(std::string name);
        void saveScene(Node* scene, std::string name, bool ovewrite = false);
        void freeScene(Node* scene);
        bool sceneExists(std::string name);
    };
}

#endif // __SCENE_MANAGER_HPP__