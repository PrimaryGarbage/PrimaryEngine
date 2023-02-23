#ifndef __NODE_HPP__
#define __NODE_HPP__

#include "node_factory.hpp"
#include <vector>
#include <stack>
#include "GLM/glm.hpp"
#include "node_path.hpp"
#include "utils.hpp"

namespace prim
{
    class Renderer;
    class SceneEditor;

    class Node
    {
        NODE_FIXTURE(Node)

    private:
        static constexpr unsigned int maxId = 100000;
        inline static bool idPool[maxId]{};
        inline static std::stack<unsigned int> freeIds;

        void updateNodePath();
    protected:

        struct StateFields
        {
            inline static const char* type = "type";
            inline static const char* name = "name";
            inline static const char* childrenStart = "children_start";
            inline static const char* childrenEnd = "children_end";
        };


        const unsigned int id;
        std::string name;
        Node* parent = nullptr;
        std::vector<Node*> children;
        NodePath nodePath;
        bool cloneBound = false;

        static unsigned int getUniqueId();
        void freeUniqueId(unsigned int id);

    public:
        Node();
        Node(std::string name);
        virtual ~Node();

        inline uint getId() const { return id; }

        virtual void start();
        virtual void update(float deltaTime);
        virtual void draw(Renderer& renderer);

        virtual glm::vec2 getPosition() const;
        virtual float getRotation() const;
        virtual glm::vec2 getScale() const;
        virtual glm::vec2 getPivot() const;
        virtual glm::vec2 getGlobalPosition() const;
        virtual float getGlobalRotation() const;
        virtual glm::vec2 getGlobalScale() const;
        virtual glm::vec2 forward() const;
        virtual glm::vec2 backward() const;
        virtual glm::vec2 left() const;
        virtual glm::vec2 right() const;
        virtual void setPosition(glm::vec2 v);
        virtual void setRotation(float angle);
        virtual void setScale(float s);
        virtual void setScale(glm::vec2 s);
        virtual void setPivot(glm::vec2 pivot);
        virtual void setGlobalPosition(glm::vec2 v);
        virtual void setGlobalRotation(float angle);
        virtual void setGlobalScale(float s);
        virtual void setGlobalScale(glm::vec2 s);

        virtual void addChild(Node* node);
        virtual void insertBefore(Node* node);
        virtual void insertAfter(Node* node);
        virtual void addSibling(Node* node);
        virtual void removeChild(Node* node);
        virtual bool hasChild(Node* node);
        virtual void orphanize();
        virtual void startChildren();
        virtual void drawChildren(Renderer& renderer);
        virtual void updateChildren(float deltaTime);
        virtual const std::vector<Node*>& getChildren() const;
        virtual const Node* getParent() const;
        virtual std::string serialize(bool withChildren = true) const;
        virtual std::string serializeChildren() const;
        virtual void deserialize(FieldValues& fieldValues);
        NodePath getNodePath() const;
        std::string getName() const;
        void setName(std::string name);
        Node* findChild(std::string name) const;
        virtual void renderFields(SceneEditor* sceneEditor);
        inline bool isBound() const noexcept { return cloneBound; };
        virtual void unbind();

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