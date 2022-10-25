#ifndef __CAMERA2D_HPP__
#define __CAMERA2D_HPP__

#include "camera_base.hpp"

namespace prim
{

    class Camera2D : public CameraBase 
    {
        NODE_FIXTURE(Camera2D)
    protected:

        struct StateFields: public Node2D::StateFields
        {
            inline static const char* zNear = "zNear";
            inline static const char* zFar = "zFar";
            inline static const char* zoom = "zoom";
        };

        glm::vec2 getWindowSize() const;
    public:
        float zNear;
        float zFar;
        float zoom = 1.0f;

        Camera2D();
        Camera2D(std::string name);
        Camera2D(std::string name, float zNear, float zFar);
        virtual ~Camera2D();

        virtual glm::mat4 calculateViewMatrix() const override;
        virtual glm::mat4 calculateProjectMatrix() const override;

        virtual void start() override;
        virtual void update(float deltaTime) override;
        virtual void draw(Renderer& renderer) override;

        virtual inline const char* type() const override { return "Camera2D"; }

        virtual std::string serialize(bool withChildren = true) const override;
        virtual void deserialize(FieldValues& fieldValues) override;
        virtual void renderFields() override;
    };

}


#endif // __CAMERA2D_HPP__