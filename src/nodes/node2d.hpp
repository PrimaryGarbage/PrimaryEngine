#ifndef __NODE2D_HPP__
#define __NODE2D_HPP__

#include "node.hpp"
#include "transform2d.hpp"
#include <string>


namespace prim
{

    class Node2D : public Node
    {
        NODE_FIXTURE(Node2D)
    protected:

        struct StateValues : public Node::StateValues
        {
            inline static const char* position = "position";
            inline static const char* rotation = "rotation";
            inline static const char* scale = "scale";
            inline static const char* pivot = "pivot";
        };

        Transform2D transform;
    public:
        using Node::Node;
        virtual ~Node2D();

        virtual void start() override;
        virtual void update(float deltaTime) override;
        virtual void draw(Renderer& renderer) override;

        void move(glm::vec2 v);
        void move(float x, float y);
        void rotate(float angle);
        void scale(float s);
        void scale(glm::vec2 s);
        void lookAt(glm::vec2 v);
        void lookAtSmooth(glm::vec2 v, float stiffness);

        virtual glm::vec2 getPosition() const override final;
        virtual float getRotation() const override final;
        virtual glm::vec2 getScale() const override final;
        virtual glm::vec2 getPivot() const override final;

        virtual glm::vec2 getGlobalPosition() const override final;
        virtual float getGlobalRotation() const override final;
        virtual glm::vec2 getGlobalScale() const override final;

        virtual glm::vec2 forward() const override final;
        virtual glm::vec2 backward() const override final;
        virtual glm::vec2 left() const override final;
        virtual glm::vec2 right() const override final;

        virtual void setPosition(glm::vec2 v) override final;
        virtual void setRotation(float angle) override final;
        virtual void setScale(float s) override final;
        virtual void setScale(glm::vec2 s) override final;
        virtual void setPivot(glm::vec2 pivot) override final;

        virtual void setGlobalPosition(glm::vec2 v) override final;
        virtual void setGlobalRotation(float angle) override final;
        virtual void setGlobalScale(float s) override final;
        virtual void setGlobalScale(glm::vec2 s) override final;

        virtual std::string serialize(bool withChildren = true) const override;
        virtual void restore(NodeValues& nodeValues) override;
        virtual void renderFields(SceneEditor* sceneEditor) override;
    };

}

#endif // __NODE2D_HPP__