#ifndef __CAMERA_BASE_HPP__
#define __CAMERA_BASE_HPP__

#include "node2d.hpp"


namespace prim
{
    class CameraBase : public Node2D
    {
    protected:
        Renderer* renderer;
    public:
        CameraBase(std::string name);
        virtual ~CameraBase();

        virtual glm::mat4 calculateViewMatrix() const = 0;
        virtual glm::mat4 calculateProjectMatrix() const = 0;
        virtual glm::vec2 worldToScreen(glm::vec3 point) = 0;
        virtual glm::vec3 screenToWorld(glm::vec2 point) = 0;
        void setAsCurrent();
        bool isCurrent() const;
    };
}


#endif // __CAMERA_BASE_HPP__