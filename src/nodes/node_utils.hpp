#ifndef __NODE_UTILS_HPP__
#define __NODE_UTILS_HPP__

#include "utils.hpp"
#include "prim_exception.hpp"

namespace prim
{
    class Node;

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