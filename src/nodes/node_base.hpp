#ifndef __NODE_BASE_HPP__
#define __NODE_BASE_HPP__

#include <stack>
#include <string>
#include <node_utils.hpp>

#define REGISTER_NODE(NODE_NAME) \
    private: inline static const NodeRegistration<NODE_NAME> nodeRegistration = NodeRegistration<NODE_NAME>(#NODE_NAME);

namespace prim
{
    class NodeBase
    {
    private:
        static constexpr unsigned int maxId = 100000;
        inline static bool idPool[maxId]{};
        inline static std::stack<unsigned int> freeIds;
    protected:
        const unsigned int id;

        static unsigned int getUniqueId();
        void freeUniqueId(unsigned int id);


    public:
        NodeBase();
        virtual ~NodeBase();

        virtual std::string serialize(bool withChildren = true) const = 0;
        virtual std::string serializeChildren() const = 0;
    };


    template<class T>
    inline static Node* instantiateNode(std::unordered_map<std::string, std::string> fieldValues)
    {
        return new T(fieldValues);
    }

    template<class T>
    struct NodeRegistration : public NodeFactory
    {
        NodeRegistration(const char* type)
        {
            node_map.insert(std::make_pair(type, &instantiateNode<T>));
        }
    };
}

#endif // __NODE_BASE_HPP__