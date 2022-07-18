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

        std::string serialize(Node* node);
        //void appendNodeToFile(Node* )
    public:
        void loadScene(const char* filePath, Node* parentNode);
        void saveScene(Node* scene, const char* filePath, bool ovewrite = false);

    };
}

#endif // __SCENE_MANAGER_HPP__