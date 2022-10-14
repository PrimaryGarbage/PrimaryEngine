#ifndef __CAMERA2D_HPP__
#define __CAMERA2D_HPP__

#include "camera_base.hpp"

namespace prim
{

    class Camera2D : public CameraBase 
    {
    protected:
        glm::vec2 getWindowSize() const;
    public:
        inline static const std::string typeName = "Camera2D";

        float zNear;
        float zFar;
        float zoom = 1.0f;

        Camera2D(std::string name);
        Camera2D(std::string name, float zNear, float zFar);
        Camera2D(std::unordered_map<std::string, std::string>& fieldValues);
        virtual ~Camera2D();

        virtual glm::mat4 calculateViewMatrix() const override;
        virtual glm::mat4 calculateProjectMatrix() const override;

        virtual void start() override;
        virtual void update(float deltaTime) override;
        virtual void draw(Renderer& renderer) override;

        virtual std::string serialize() const override;
        virtual inline const std::string getTypeName() const override { return typeName; }
        virtual void renderFields() override;
    };

}


#endif // __CAMERA2D_HPP__