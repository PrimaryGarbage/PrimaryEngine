#ifndef __ACTOR_CAMERA2D_HPP__
#define __ACTOR_CAMERA2D_HPP__

#include "camera2d.hpp"

namespace prim
{
    class ActorCamera2D : public Camera2D
    {
        REGISTER_NODE(ActorCamera2D)
    protected:
        Node2D* target;
        NodePath targetPath;
        glm::vec2 initialOffset { 0.0f, 0.0f };
        float stiffness = 1.0f;
        
    public:
        bool rotateWithTarget = true;

        ActorCamera2D(std::string name, const NodePath& target);
        ActorCamera2D(std::string name, float zNear, float zFar, const NodePath& target);
        ActorCamera2D(std::unordered_map<std::string, std::string>& fieldValues);
        virtual ~ActorCamera2D();

        virtual void start() override;
        virtual void update(float deltaTime) override;
        virtual void draw(Renderer& renderer) override;

        virtual inline const char* type() const override { return "Camera2D"; }

        void setTarget(const NodePath& target);
        inline NodePath getTarget() const { return targetPath; }
        void setStiffness(float value);
        inline float getStiffness() const { return stiffness; }
        inline glm::vec2 getInitialOffset() const { return initialOffset; }

        virtual std::string serialize(bool withChildren = true) const override;
        virtual void renderFields() override;
    };
}


#endif // __ACTOR_CAMERA2D_HPP__