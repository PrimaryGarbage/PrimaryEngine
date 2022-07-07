#ifndef __NODE_HPP__
#define __NODE_HPP__

#include <vector>
#include <string>
#include "node_base.hpp"

namespace prim
{

    class Node : public NodeBase
    {
    private:
        Node* parent = nullptr;
        std::vector<Node*> children;

    public:
        std::string name;

        Node(std::string name);
        virtual ~Node();

        virtual void start();
        virtual void update(float deltaTime);

        void addChild(Node* node);
        void removeChild(Node* node);
        const std::vector<Node*> getChildren() const;
    };

}

#endif // __NODE_HPP__