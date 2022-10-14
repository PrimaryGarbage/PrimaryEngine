#ifndef __NODE_UTILS_HPP__
#define __NODE_UTILS_HPP__

#include "utils.hpp"
#include "prim_exception.hpp"

namespace prim
{
    class Node;

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

    class NodeFactory
    {
    protected:
        static inline std::unordered_map<std::string, Node* (*)(std::unordered_map<std::string, std::string>)> node_map;

    public:
        static inline Node* createNode(std::string type, std::unordered_map<std::string, std::string> fieldValues)
        {
            auto iter = node_map.find(type);
            if(iter == node_map.end()) throw PRIM_EXCEPTION("Couldn't find node type '" + type + "'");
            return iter->second(fieldValues);
        }
    };

}

#endif // __NODE_UTILS_HPP__