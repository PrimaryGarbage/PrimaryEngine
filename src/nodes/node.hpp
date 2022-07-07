#ifndef __NODE_HPP__
#define __NODE_HPP__

#include <vector>
#include <string>
#include "node_base.hpp"
#include "glm.hpp"

namespace prim
{
    class Renderer;

    class Node : public NodeBase
    {
    private:
    protected:
        Node* parent = nullptr;
        std::vector<Node*> children;

    public:
        std::string name;

        Node(std::string name);
        virtual ~Node();

        virtual void start();
        virtual void update(float deltaTime);
        virtual void draw(Renderer& renderer);

        virtual void addChild(Node* node);
        virtual void removeChild(Node* node);
        virtual const std::vector<Node*>& getChildren() const;
    };

}

#endif // __NODE_HPP__