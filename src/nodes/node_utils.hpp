#ifndef __NODE_UTILS_HPP__
#define __NODE_UTILS_HPP__

#include "node.hpp"
#include "node2d.hpp"
#include "sprite.hpp"
#include "camera2d.hpp"
#include "actor_camera2d.hpp"
#include "utils.hpp"


namespace prim
{
    const std::unordered_map<std::string, int> nodeTypeMap = {
        { Node::typeName, 0 },
        { Node2D::typeName,  1 },
        { Sprite::typeName, 2 },
        { Camera2D::typeName, 3 },
        { ActorCamera2D::typeName, 4 }
    };


    struct NodeFields
    {
        inline static const char* header = "Node";
        inline static const char* type = "type";
        inline static const char* name = "name";
        inline static const char* childrenStart = "children_start";
        inline static const char* childrenEnd = "children_end";
        inline static const char* position = "position";
        inline static const char* rotation = "rotation";
        inline static const char* scale = "scale";
        inline static const char* pivot = "pivot";
        inline static const char* width = "width";
        inline static const char* height = "height";
        inline static const char* zIndex = "zIndex";
        inline static const char* imagePath = "imagePath";
        inline static const char* zNear = "zNear";
        inline static const char* zFar = "zFar";
        inline static const char* zoom = "zoom";
        inline static const char* targetPath = "targetPath";
        inline static const char* initialOffset = "initialOffset";
        inline static const char* stiffness = "stiffness";
        inline static const char* rotateWithTarget = "rotateWithTarget";
    };

    template<class... Args>
    inline Node* createNode(const char* type, const char* name, Args&&... args)
    {
        switch (nodeTypeMap.at(type))
        {
        case 0: // Node
            return new Node(name);
        case 1: // Node2D
            return new Node2D(name);
        case 2: // Sprite
            return new Sprite(name, std::forward<Args>(args)...);
        case 3: // Camera2D
            return new Camera2D(name, std::forward<Args>(args)...);
        case 4: // ActorCamera2D
            return new ActorCamera2D(name, std::forward<Args>(args)...);
        default:
            return nullptr;
        }
    }

    inline Node* createNode(const char* type, std::unordered_map<std::string, std::string>& fieldValues)
    {
        switch (nodeTypeMap.at(type))
        {
        case 0: // Node
            return new Node(fieldValues);
        case 1: // Node2D
            return new Node2D(fieldValues);
        case 2: // Sprite
            return new Sprite(fieldValues);
        case 3: // Camera2D
            return new Camera2D(fieldValues);
        case 4: // ActorCamera2D
            return new ActorCamera2D(fieldValues);
        default:
            return nullptr;
        }
    }

}

#endif // __NODE_UTILS_HPP__