#ifndef __ACTOR_CAMERA2D_HPP__
#define __ACTOR_CAMERA2D_HPP__

#include "camera2d.hpp"

namespace prim
{
    class ActorCamera2D : public Camera2D
    {
        NODE_FIXTURE(ActorCamera2D)
    protected:

        struct StateValues: public Camera2D::StateValues
        {
            inline static const char* targetPath = "targetPath";
            inline static const char* initialOffset = "initialOffset";
            inline static const char* stiffness = "stiffness";
            inline static const char* rotateWithTarget = "rotateWithTarget";
        };

        Node2D* target = nullptr;
        NodePath targetPath;
        glm::vec2 initialOffset { 0.0f, 0.0f };
        float stiffness = 1.0f;
        
    public:
        bool rotateWithTarget = true;

        ActorCamera2D();
        ActorCamera2D(std::string name);
        ActorCamera2D(std::string name, const NodePath& target);
        ActorCamera2D(std::string name, float zNear, float zFar, const NodePath& target);
        virtual ~ActorCamera2D();

        virtual void start() override;
        virtual void update(float deltaTime) override;
        virtual void draw(Renderer& renderer) override;

        void setTarget(const NodePath& target);
        inline NodePath getTarget() const { return targetPath; }
        void setStiffness(float value);
        inline float getStiffness() const { return stiffness; }
        inline glm::vec2 getInitialOffset() const { return initialOffset; }

        virtual std::string serialize(bool withChildren = true) const override;
        virtual void restore(NodeValues& nodeValues) override;
        virtual void renderFields(SceneEditor* sceneEditor) override;
    };
}


#endif // __ACTOR_CAMERA2D_HPP__