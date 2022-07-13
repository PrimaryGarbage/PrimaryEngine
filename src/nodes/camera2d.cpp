#include "camera2d.hpp"
#include "renderer.hpp"
#include "gtc/matrix_transform.hpp"
#include "utils.hpp"

namespace prim
{

    Camera2D::Camera2D(std::string name, Renderer* renderer)
        : Camera2D(name, renderer, -1.0f, 1.0f)
    {
    }
    
    Camera2D::Camera2D(std::string name, Renderer* renderer, float zNear, float zFar)
        : CameraBase(name, renderer), zNear(zNear), zFar(zFar)
    {
        setAsCurrent();
        glm::vec2 windowSize = getWindowSize();
        setPivot(glm::vec2(0.5f, 0.5f));
    }
    
    Camera2D::~Camera2D()
    {
    }
    
    glm::vec2 Camera2D::getWindowSize() const 
    {
        return glm::vec2(static_cast<float>(renderer->getWindowWidth()), static_cast<float>(renderer->getWindowHeight()));
    }

    glm::mat4 Camera2D::calculateViewMatrix() const 
    {
        glm::mat4 viewMatrix(1.0f);
        viewMatrix = glm::translate(viewMatrix, Utils::toVec3(getPivot() * getWindowSize()));
        viewMatrix = glm::rotate(viewMatrix, -getGlobalRotation(), glm::vec3(0.0f, 0.0f, 1.0f));
        viewMatrix = glm::translate(viewMatrix, Utils::toVec3(-getGlobalPosition()));
        return std::move(viewMatrix);
    }

    glm::mat4 Camera2D::calculateProjectMatrix() const
    {
        glm::vec2 windowSize = getWindowSize();
        return std::move(glm::ortho(0.0f, windowSize.x, 0.0f, windowSize.y, zNear, zFar));
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