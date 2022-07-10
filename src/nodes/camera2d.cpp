#include "camera2d.hpp"
#include "renderer.hpp"
#include "gtc/matrix_transform.hpp"
#include "utils.hpp"

namespace prim
{
    Camera2D::Camera2D(std::string name, Renderer* renderer)
        : Camera2D(name, renderer, static_cast<float>(renderer->getWindowWidth()), static_cast<float>(renderer->getWindowHeight()), -1.0f, 1.0f)
    {
    }
    
    Camera2D::Camera2D(std::string name, Renderer* renderer, float width, float height, float zNear, float zFar)
        : CameraBase(name, renderer), width(width), height(height), zNear(zNear), zFar(zFar)
    {
        setAsCurrent();
    }
    
    Camera2D::~Camera2D()
    {
    }
    
    glm::mat4 Camera2D::calculateViewMatrix() const 
    {
        glm::mat4 viewMatrix(1.0f);
        viewMatrix = glm::rotate(viewMatrix, -getGlobalRotation(), glm::vec3(0.0f, 0.0f, 1.0f));
        viewMatrix = glm::translate(viewMatrix,Utils::toVec3(-getGlobalPosition()));
        return std::move(viewMatrix);
    }

    glm::mat4 Camera2D::calculateProjectMatrix() const
    {
        return std::move(glm::ortho(0.0f, width, 0.0f, height, zNear, zFar));
    }
    
    void Camera2D::start()
    {
        START_CHILDREN
    }
    
    void Camera2D::update(float deltaTime)
    {
        UPDATE_CHILDREN
    }
    
    void Camera2D::draw(Renderer& renderer)
    {
        DRAW_CHILDREN
    }

}