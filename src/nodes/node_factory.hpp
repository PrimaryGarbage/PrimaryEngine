#ifndef __NODE_FACTORY_HPP__
#define __NODE_FACTORY_HPP__

#include <unordered_map>
#include <string>
#include <vector>
#include "prim_exception.hpp"
#include "typedefs.hpp"

#define REGISTER_NODE(NODE_NAME) \
    private: inline static const NodeRegistration<NODE_NAME> nodeRegistration = NodeRegistration<NODE_NAME>(#NODE_NAME);

namespace prim
{
    class Node;

    // helper function. Serves as a generic node factory
    template<class T>
    inline static Node* constructNode()
    {
        return new T();
    }

    class NodeFactory
    {
    protected:
        static inline std::unordered_map<std::string, Node* (*)()> node_map;

    public:
        static inline Node* createNode(std::string type)
        {
            auto iter = node_map.find(type);
            if(iter == node_map.end()) throw PRIM_EXCEPTION("Couldn't find node type '" + type + "'");
            Node* newNode = iter->second();
            return newNode;
        }

        static inline std::vector<std::string> getAllNodeTypes()
        {
            std::vector<std::string> result;
            result.reserve(node_map.size());
            for(const auto& pair : node_map)
                result.push_back(pair.first);
            return result;
        }
    };

    // object of this class registers it's node on instantiation
    template<class T>
    struct NodeRegistration : public NodeFactory
    {
        NodeRegistration(const char* type)
        {
            node_map.insert(std::make_pair(type, &constructNode<T>));
        }
    };

}

#endif // __NODE_FACTORY_HPP__