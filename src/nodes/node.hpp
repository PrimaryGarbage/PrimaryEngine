#ifndef __NODE_HPP__
#define __NODE_HPP__

#include <vector>
#include <unordered_map>
#include "node_base.hpp"
#include "glm.hpp"

namespace prim
{
#define START_CHILDREN for(Node* child : children) child->start();
#define UPDATE_CHILDREN for(Node* child : children) child->update(deltaTime);
#define DRAW_CHILDREN for(Node* child : children) child->draw(renderer);

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
        Node(std::unordered_map<std::string, std::string>& fieldValues);
        virtual ~Node();

        virtual void start();
        virtual void update(float deltaTime);
        virtual void draw(Renderer& renderer);

        virtual void addChild(Node* node);
        virtual void removeChild(Node* node);
        virtual const std::vector<Node*>& getChildren() const;

        virtual std::string serialize() const override;

        template<class T>
        T* findChild(std::string name)
        {
            T* childT = nullptr;
            for (Node* child : children)
            {
                childT = dynamic_cast<T*>(child);
                if (childT)
                {
                    if (childT->name == name)
                        return childT;
                    else
                        childT = nullptr;
                }

                childT = child->findChild<T>(name);
                if(childT) return childT;
            }

            return childT;
        }

        Node* findChild(std::string name)
        {
            Node* child = nullptr;
            for (Node* child : children)
            {
                if (child->name == name)
                    return child;

                child = child->findChild(name);
                if(child) return child;
            }

            return child;
        }

    };

}

#endif // __NODE_HPP__