#ifndef __NODE_HPP__
#define __NODE_HPP__

#include <vector>
#include <unordered_map>
#include "node_base.hpp"
#include "glm.hpp"
#include "node_path.hpp"

namespace prim
{
    class Renderer;

    class Node : public NodeBase
    {
        REGISTER_NODE(Node)

    private:
        void updateNodePath();
    protected:

        struct StateFields
        {
            inline static const char* type = "type";
            inline static const char* name = "name";
            inline static const char* childrenStart = "children_start";
            inline static const char* childrenEnd = "children_end";
        };

        std::string name;
        Node* parent = nullptr;
        std::vector<Node*> children;
        NodePath nodePath;

    public:
        Node(std::string name);
        Node(FieldValues& fieldValues);
        virtual ~Node();

        virtual void start();
        virtual void update(float deltaTime);
        virtual void draw(Renderer& renderer);

        virtual inline const char* type() const { return "Node"; }

        virtual void addChild(Node* node);
        virtual void removeChild(Node* node);
        virtual void orphanize();
        virtual void startChildren();
        virtual void drawChildren(Renderer& renderer);
        virtual void updateChildren(float deltaTime);
        virtual const std::vector<Node*>& getChildren() const;
        virtual const Node* getParent() const;
        virtual std::string serialize(bool withChildren = true) const override;
        virtual std::string serializeChildren() const override;
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