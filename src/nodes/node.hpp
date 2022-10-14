#ifndef __NODE_HPP__
#define __NODE_HPP__

#include <vector>
#include <unordered_map>
#include "node_base.hpp"
#include "glm.hpp"
#include "node_path.hpp"

namespace prim
{

#define START_CHILDREN for(Node* child : children) child->start();
#define UPDATE_CHILDREN for(Node* child : children) child->update(deltaTime);
#define DRAW_CHILDREN for(Node* child : children) child->draw(renderer);

    class Renderer;

    class Node : public NodeBase
    {
        REGISTER_NODE(Node)

        void updateNodePath();
    protected:
        std::string name;
        Node* parent = nullptr;
        std::vector<Node*> children;
        NodePath nodePath;

    public:
        inline static const std::string typeName = "Node";

        Node(std::string name);
        Node(std::unordered_map<std::string, std::string>& fieldValues);
        virtual ~Node();

        virtual void start();
        virtual void update(float deltaTime);
        virtual void draw(Renderer& renderer);

        virtual void addChild(Node* node);
        virtual void removeChild(Node* node);
        virtual void orphanize();
        virtual const std::vector<Node*>& getChildren() const;
        virtual const Node* getParent() const;
        virtual std::string serialize() const override;
        virtual inline const std::string getTypeName() const { return typeName; }
        NodePath getNodePath() const;
        std::string getName() const;
        void setName(std::string name);
        Node* findChild(std::string name) const;
        virtual void renderFields();

        template<class T>
        T* findChild(std::string name = "") const
        {
            T* childT = nullptr;
            for (Node* child : children)
            {
                childT = dynamic_cast<T*>(child);
                if (childT)
                {
                    if (!name.empty())
                        if (childT->name == name)
                            return childT;
                        else
                            childT = nullptr;
                    else
                        return childT;
                }

                childT = child->findChild<T>(name);
                if (childT) return childT;
            }

            return childT;
        }
    };

}

#endif // __NODE_HPP__