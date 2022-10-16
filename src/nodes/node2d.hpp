#ifndef __NODE2D_HPP__
#define __NODE2D_HPP__

#include "node.hpp"
#include "transform2d.hpp"


namespace prim
{

    class Node2D : public Node
    {
        REGISTER_NODE(Node2D)
    protected:

        struct StateFields: public Node::StateFields
        {
            inline static const char* position = "position";
            inline static const char* rotation = "rotation";
            inline static const char* scale = "scale";
            inline static const char* pivot = "pivot";
        };

        Transform2D transform;
    public:
        Node2D(std::string name);
        Node2D(FieldValues& fieldValues);
        virtual ~Node2D();

        virtual void start() override;
        virtual void update(float deltaTime) override;
        virtual void draw(Renderer& renderer) override;

        virtual inline const char* type() const override { return "Node2D"; }
        
        void move(glm::vec2 v);
        void move(float x, float y);
        void rotate(float angle);
        void scale(float s);
        void scale(glm::vec2 s);
        void lookAt(glm::vec2 v);
        void lookAtSmooth(glm::vec2 v, float stiffness);

        glm::vec2 getPosition() const;
        float getRotation() const;
        glm::vec2 getScale() const;
        glm::vec2 getPivot() const;

        glm::vec2 getGlobalPosition() const;
        float getGlobalRotation() const;
        glm::vec2 getGlobalScale() const;

        glm::vec2 forward() const;
        glm::vec2 backward() const;
        glm::vec2 left() const;
        glm::vec2 right() const;

        void setPosition(glm::vec2 v);
        void setRotation(float angle);
        void setScale(float s);
        void setScale(glm::vec2 s);
        void setPivot(glm::vec2 pivot);

        void setGlobalPosition(glm::vec2 v);
        void setGlobalRotation(float angle);
        void setGlobalScale(float s);
        void setGlobalScale(glm::vec2 s);

        virtual std::string serialize(bool withChildren = true) const override;
        virtual void renderFields() override;
    };

}

#endif // __NODE2D_HPP__