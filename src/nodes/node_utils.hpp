#ifndef __NODE_UTILS_HPP__
#define __NODE_UTILS_HPP__

#include <unordered_map>
#include "node.hpp"
#include "node2d.hpp"
#include "sprite.hpp"
#include "camera2d.hpp"
#include "actor_camera2d.hpp"
#include <sstream>

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

    struct NodeFields
    {
        inline static const char* header = "Node";
        inline static const char* type = "type";
        inline static const char* name = "name";
        inline static const char* children = "children";
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
        inline static const char* target = "target";
        inline static const char* initialOffset = "initialOffset";
        inline static const char* stiffness = "stiffness";
        inline static const char* rotateWithTarget = "rotateWithTarget";
    };

    template<class... Args>
    Node* createNode(const char* type, const char* name, Args&&... args)
    {
        switch (nodeTypeMap.at(type))
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

    inline std::string createKeyValuePair(std::string key, std::string value)
    {
        static const char keyValueSeparator = '=';
        return std::string(std::move(key) + keyValueSeparator + std::move(value));
    }

    inline std::string serializeVec2(const glm::vec2& vec)
    {
        return std::to_string(vec.x) + ',' + std::to_string(vec.y);
    }
}

#endif // __NODE_UTILS_HPP__