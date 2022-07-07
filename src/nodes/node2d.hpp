#ifndef __NODE2D_HPP__
#define __NODE2D_HPP__

#include "node.hpp"
#include "transform2d.hpp"


namespace prim
{

    class Node2D : public Node
    {
    protected:
        Transform2D transform;
    public:
        Node2D(std::string name);
        virtual ~Node2D();

        virtual void start() override;
        virtual void update(float deltaTime) override;
        virtual void draw() override;

        void move(glm::vec2 v);
        void rotate(float angle);
        void scale(float s);
        void scale(glm::vec2 s);

        glm::vec2 getPosition();
        float getRotation();
        glm::vec2 getScale();

        glm::vec2 getGlobalPosition();
        float getGlobalRotation();
        glm::vec2 getGlobalScale();

        void setPosition(glm::vec2 v);
        void setRotation(float angle);
        void setScale(float s);
        void setScale(glm::vec2 s);

        void setGlobalPosition(glm::vec2 v);
        void setGlobalRotation(float angle);
        void setGlobalScale(float s);
        void setGlobalScale(glm::vec2 s);
    };

}

#endif // __NODE2D_HPP__