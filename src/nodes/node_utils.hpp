#ifndef __NODE_UTILS_HPP__
#define __NODE_UTILS_HPP__

#include <unordered_map>
#include "node.hpp"
#include "node2d.hpp"
#include "sprite.hpp"
#include "camera2d.hpp"
#include "actor_camera2d.hpp"

namespace prim
{
    template<class T>
    inline const char* getNodeTypeName() { return "unknown node type"; }
    template<>
    inline const char* getNodeTypeName<Node>() { return "Node"; }
    template<>
    inline const char* getNodeTypeName<Node2D>() { return "Node2D"; }
    template<>
    inline const char* getNodeTypeName<Sprite>() { return "Sprite"; }
    template<>
    inline const char* getNodeTypeName<Camera2D>() { return "Camera2D"; }
    template<>
    inline const char* getNodeTypeName<ActorCamera2D>() { return "ActorCamera2D"; }

    static const std::unordered_map<std::string, int> nodeTypeMap = {
        { getNodeTypeName<Node>(), 0 },
        { getNodeTypeName<Node2D>(),  1 },
        { getNodeTypeName<Sprite>(), 2 },
        { getNodeTypeName<Camera2D>(), 3 },
        { getNodeTypeName<ActorCamera2D>(), 4 }
    };

    template<class... Args>
    Node* createNode(const char* type, const char* name, Args&&... args)
    {
        switch (nodeTypeMap[type])
        {
            case 0:
                return new Node(name);
            case 1:
                return new Node2D(name);
            case 2: 
                return new Sprite(name, std::forward<Args>(args)...);
            case 3:
                return new Camera2D(name, std::forward<Args>(args)...);
            case 4:
                return new ActorCamera2D(name, std::forward<Args>(args)...);
            default:
                return nullptr;
        }
    }
}

#endif // __NODE_UTILS_HPP__