#ifndef __CAMERA_BASE_HPP__
#define __CAMERA_BASE_HPP__

#include "node2d.hpp"


namespace prim
{
    class CameraBase : public Node2D
    {
    protected:
        Renderer* renderer;
        bool current = false;
    public:
        inline static const std::string typeName = "CameraBase";

        CameraBase(std::string name);
        virtual ~CameraBase();

        virtual glm::mat4 calculateViewMatrix() const = 0;
        virtual glm::mat4 calculateProjectMatrix() const = 0;
        virtual void start() override = 0;
        virtual void update(float deltaTime) override = 0;
        virtual void draw(Renderer& renderer) override = 0;
        virtual void setAsCurrent();
        inline virtual bool isCurrent() const { return current; }
        virtual inline const std::string getTypeName() const override { return typeName; }
    };
}


#endif // __CAMERA_BASE_HPP__