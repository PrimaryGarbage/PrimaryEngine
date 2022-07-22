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
        inline static const std::string typeName = "Node2D";

        Node2D(std::string name);
        Node2D(std::unordered_map<std::string, std::string>& fieldValues);
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

        virtual std::string serialize() const override;
        virtual inline const std::string getTypeName() const override { return typeName; }
        virtual void visualizeOnUi() override;
    };

}

#endif // __NODE2D_HPP__