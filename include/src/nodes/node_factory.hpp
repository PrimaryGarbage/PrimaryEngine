#ifndef __NODE_FACTORY_HPP__
#define __NODE_FACTORY_HPP__

#include <unordered_map>
#include <string>
#include <vector>
#include "src/prim_exception.hpp"
#include "src/typedefs.hpp"

#define NODE_FIXTURE(NODE_NAME) \
    private: inline static const NodeRegistration<NODE_NAME> nodeRegistration = NodeRegistration<NODE_NAME>(#NODE_NAME); \
    public: inline virtual Node* clone() \
    { \
        NODE_NAME* cloned = new NODE_NAME(*this); \
        cloned->children.clear(); \
        for(Node* child : children) \
        {   \
            Node* clonedChild = child->clone(); \
            clonedChild->orphanize(); \
            cloned->addChild(clonedChild); \
        }   \
        cloned->cloneBound = true; \
        return cloned; \
    } \
    public: virtual inline const char* type() const { return #NODE_NAME; }

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
        static inline std::unordered_map<std::string, Node* (*)()> construct_node_map;

    public:
        static inline Node* createNode(std::string type)
        {
            auto iter = construct_node_map.find(type);
            if(iter == construct_node_map.end()) throw PRIM_EXCEPTION("Couldn't find node type '" + type + "'");
            Node* newNode = iter->second();
            return newNode;
        }

        static inline std::vector<std::string> getAllNodeTypes()
        {
            std::vector<std::string> result;
            result.reserve(construct_node_map.size());
            for(const auto& pair : construct_node_map)
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
            construct_node_map.insert(std::make_pair(type, &constructNode<T>));
        }
    };

}

#endif // __NODE_FACTORY_HPP__