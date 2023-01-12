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

        const static inline char keyValueSeparator = '=';

        std::string serialize(Node* node);
    public:
        const static inline char* sceneFileExtension = ".psc";

        Node* loadScene(std::string resPath);
        void saveScene(Node* scene, std::string resPath, bool ovewrite = false);
        void freeScene(Node* scene);
        Node* createEmptyScene() const;
    };
}

#endif // __SCENE_MANAGER_HPP__