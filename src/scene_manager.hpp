#ifndef __SCENE_MANAGER_HPP__
#define __SCENE_MANAGER_HPP__

#include <string>

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
        //void appendNodeToFile(Node* )
    public:
        SceneManager(const char* savePath);

        void loadScene(std::string fileName, Node* parentNode);
        void saveScene(Node* scene, std::string fileName, bool ovewrite = false);

    };
}

#endif // __SCENE_MANAGER_HPP__