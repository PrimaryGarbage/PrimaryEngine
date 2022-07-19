#ifndef __CAMERA2D_HPP__
#define __CAMERA2D_HPP__

#include "camera_base.hpp"

namespace prim
{

    class Camera2D : public CameraBase 
    {
    private:
        glm::vec2 getWindowSize() const;
    public:
        float zNear;
        float zFar;
        float zoom = 1.0f;

        Camera2D(std::string name, Renderer* renderer);
        Camera2D(std::string name, Renderer* renderer, float zNear, float zFar);
        virtual ~Camera2D();

        virtual glm::mat4 calculateViewMatrix() const override;
        virtual glm::mat4 calculateProjectMatrix() const override;

        virtual void start() override;
        virtual void update(float deltaTime) override;
        virtual void draw(Renderer& renderer) override;

        virtual std::string serialize() const override;
    };

}


#endif // __CAMERA2D_HPP__