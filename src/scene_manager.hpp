#ifndef __SCENE_MANAGER_HPP__
#define __SCENE_MANAGER_HPP__

#include <string>
#include <filesystem>
#include <vector>
#include "typedefs.hpp"

namespace prim
{
    class Node;

    class SceneManager
    {
    public:
        const static inline char* sceneFileExtension = ".psc";

        Node* loadScene(std::string resPath) const;
        void saveScene(Node* scene, std::string resPath, bool ovewrite = false) const;
        void saveScene(const std::vector<Node*>& scene, std::string resPath, bool ovewrite = false) const;
        void freeScene(Node* scene) const;
        void freeScene(std::vector<Node*>& scene) const;
    };
}

#endif // __SCENE_MANAGER_HPP__