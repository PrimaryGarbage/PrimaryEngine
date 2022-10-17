#ifndef __NODE_UTILS_HPP__
#define __NODE_UTILS_HPP__

#include "prim_exception.hpp"
#include "node.hpp"
#include <algorithm>

namespace prim
{
    inline std::string generateNodeName(Node* node)
    {
        // const Node* parent = node->getParent();
        // if(parent)
        // {
        //     int idx = 0;
        //     std::string name = node->type();
        //     const auto& siblings = node->getParent()->getChildren();
        //     while(std::find_if(siblings.begin(), siblings.end(), 
        //         [&name, &idx] (const Node* sib) { return sib->getName() == name + std::to_string(idx); })
        //         != siblings.end())
        //         ++idx;
        //     return name + std::to_string(idx);
        // }
        // else
        // {
        //     return node->type();
        // }

        return "no_name";
    }

    inline std::string createNodeSignature(Node* node)
    {
        return std::string(node->type()) + ": '" + node->getName() + "'";
    }
}

#endif // __NODE_UTILS_HPP__