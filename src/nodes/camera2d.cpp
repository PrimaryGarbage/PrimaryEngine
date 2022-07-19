#include "camera2d.hpp"
#include "renderer.hpp"
#include "gtc/matrix_transform.hpp"
#include "utils.hpp"
#include "node_utils.hpp"

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
        viewMatrix = glm::translate(viewMatrix, Utils::toVec3(getPivot() * getWindowSize(), -0.7f));
        viewMatrix = glm::rotate(viewMatrix, -getGlobalRotation(), glm::vec3(0.0f, 0.0f, 1.0f));
        viewMatrix = glm::scale(viewMatrix, glm::vec3(zoom, zoom, 1.0f));
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

    std::string Camera2D::serialize() const
    {
        std::stringstream ss;
        ss << NodeFields::header << std::endl;
        ss << createKeyValuePair(NodeFields::type, getNodeTypeName<Camera2D>()) << std::endl;
        ss << createKeyValuePair(NodeFields::name, name) << std::endl;
        ss << createKeyValuePair(NodeFields::position, serializeVec2(getPosition())) << std::endl;
        ss << createKeyValuePair(NodeFields::rotation, std::to_string(getRotation())) << std::endl;
        ss << createKeyValuePair(NodeFields::scale, serializeVec2(getScale())) << std::endl;
        ss << createKeyValuePair(NodeFields::pivot, serializeVec2(getPivot())) << std::endl;
        ss << createKeyValuePair(NodeFields::zNear, std::to_string(zNear)) << std::endl;
        ss << createKeyValuePair(NodeFields::zFar, std::to_string(zFar)) << std::endl;
        ss << createKeyValuePair(NodeFields::zoom, std::to_string(zoom)) << std::endl;
        ss << NodeFields::children << std::endl;
        ss << "{\n";
        for (Node* child : children)
            ss << child->serialize() << std::endl;
        ss << "}\n";
        return ss.str();
    }

}