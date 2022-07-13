#ifndef __ACTOR_CAMERA2D_HPP__
#define __ACTOR_CAMERA2D_HPP__

#include "camera2d.hpp"

namespace prim
{
    class ActorCamera2D : public Camera2D
    {
    private:
        Node2D* target;
        glm::vec2 initialOffset { 0.0f, 0.0f };
    public:
        float smoothness = 1.0f;
        bool rotateWithTarget = true;

        ActorCamera2D(std::string name, Renderer* renderer, Node2D* target);
        ActorCamera2D(std::string name, Renderer* renderer, float zNear, float zFar, Node2D* target);
        virtual ~ActorCamera2D();

        virtual void start() override;
        virtual void update(float deltaTime) override;
        virtual void draw(Renderer& renderer) override;

        void setTarget(Node2D* target);
        inline Node2D* getTarget() const { return target; }
    };
}


#endif // __ACTOR_CAMERA2D_HPP__