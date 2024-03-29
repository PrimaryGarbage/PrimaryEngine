#ifndef __CAMERA2D_HPP__
#define __CAMERA2D_HPP__

#include "camera_base.hpp"

namespace prim
{

    class Camera2D : public CameraBase 
    {
        NODE_FIXTURE(Camera2D)
    protected:

        struct StateValues: public Node2D::StateValues
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

        virtual glm::mat4 calculateViewMatrix() const override;
        virtual glm::mat4 calculateProjectMatrix() const override;

        virtual std::string serialize(bool withChildren = true) const override;
        virtual void restore(NodeValues& nodeValues) override;
        virtual void renderFields(SceneEditor* sceneEditor) override;

        virtual glm::vec2 worldToScreen(glm::vec3 point) override;
        virtual glm::vec3 screenToWorld(glm::vec2 point) override;
    };

}


#endif // __CAMERA2D_HPP__