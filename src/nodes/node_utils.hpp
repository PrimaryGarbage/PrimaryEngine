#ifndef __NODE_UTILS_HPP__
#define __NODE_UTILS_HPP__

#include <unordered_map>
#include "node.hpp"
#include "node2d.hpp"
#include "sprite.hpp"
#include "camera2d.hpp"
#include "actor_camera2d.hpp"
#include "utils.hpp"
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

    static const char keyValueSeparator = '=';
    static const char vecSeparator = ',';

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

    inline Node* createNode(const char* type, std::unordered_map<std::string, std::string>& fieldValues)
    {
        switch (nodeTypeMap.at(type))
        {
        case 0:
            return new Node(fieldValues);
        case 1:
            return new Node2D(fieldValues);
        case 2:
            return new Sprite(fieldValues);
        case 3:
            return new Camera2D(fieldValues);
        case 4:
            return new ActorCamera2D(fieldValues);
        default:
            return nullptr;
        }
    }

    inline std::string createKeyValuePair(std::string key, std::string value)
    {
        return std::string(std::move(key) + keyValueSeparator + std::move(value));
    }

    inline std::pair<std::string, std::string> parseKeyValuePair(const std::string& line)
    {
        size_t pos = line.find(keyValueSeparator);
        std::pair<std::string, std::string> result(line.substr(0, pos), line.substr(pos + 1));
        return std::pair(line.substr(0, pos), line.substr(pos + 1));
    }

    inline std::string serializeVec2(const glm::vec2& vec)
    {
        return std::to_string(vec.x) + vecSeparator + std::to_string(vec.y);
    }

    inline glm::vec2 deserializeVec2(const std::string& line)
    {
        std::vector<std::string> values = Utils::splitString(line, vecSeparator);
        return glm::vec2(std::stof(values[0]), std::stof(values[1]));
    }
}

#endif // __NODE_UTILS_HPP__