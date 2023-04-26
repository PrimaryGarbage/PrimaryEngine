#ifndef __CONTROL_HPP__
#define __CONTROL_HPP__

#include "node.hpp"
#include "control_transform.hpp"

namespace prim
{

    class Control : public Node
    {
        NODE_FIXTURE(Control)

    protected:
        struct StateValues : public Node::StateValues
        {
            inline static const char* position = "position";
            inline static const char* rotation = "rotation";
            inline static const char* size = "size";
            inline static const char* pivot = "pivot";
            inline static const char* anchor = "anchor";
        };

        ControlTransform transform;
        Control* controlParent = nullptr;
    public:
        Control();
        Control(std::string name);

        void move(glm::vec2 v);
        void move(float x, float y);
        void rotate(float angle);

        virtual glm::vec2 getPosition() const override final;
        virtual float getRotation() const override final;
        virtual glm::vec2 getSize() const;
        virtual glm::vec2 getPivot() const override final;
        virtual glm::vec2 getAnchor() const;

        virtual glm::vec2 getGlobalPosition() const override final;
        virtual float getGlobalRotation() const override final;

        virtual void setPosition(glm::vec2 v) override final;
        virtual void setRotation(float angle) override final;
        virtual void setSize(glm::vec2 s);
        virtual void setSize(float width, float height);
        virtual void setWidth(float width);
        virtual void setHeight(float height);
        virtual void setPivot(glm::vec2 pivot) override final;
        virtual void setAnchor(glm::vec2 anchor);

        virtual void setGlobalPosition(glm::vec2 v) override final;
        virtual void setGlobalRotation(float angle) override final;

        virtual void addChild(Node* node) override;
        virtual void insertBefore(Node* node) override;
        virtual void insertAfter(Node* node) override;
        virtual void addSibling(Node* node) override;
        virtual void removeChild(Node* node) override;
        virtual bool hasChild(Node* node) override;
        virtual void orphanize() override;

        virtual std::string serialize(bool withChildren = true) const override;
        virtual void restore(NodeValues& nodeValues) override;
        virtual void renderFields(SceneEditor* sceneEditor) override;
    };
}

#endif // __CONTROL_HPP__