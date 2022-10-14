#include "camera2d.hpp"
#include "renderer.hpp"
#include "gtc/matrix_transform.hpp"
#include "utils.hpp"
#include "node_utils.hpp"
#include "imgui.h"

namespace prim
{

    Camera2D::Camera2D(std::string)
        : Camera2D(name, -1.0f, 1.0f)
    {
    }

    Camera2D::Camera2D(std::string name, float zNear, float zFar)
        : CameraBase(name), zNear(zNear), zFar(zFar)
    {
        setAsCurrent();
        glm::vec2 windowSize = getWindowSize();
        setPivot(glm::vec2(0.5f, 0.5f));
    }
    
    Camera2D::Camera2D(std::unordered_map<std::string, std::string>& fieldValues) 
        : Camera2D(fieldValues[NodeFields::name], std::stof(fieldValues[NodeFields::zNear]), std::stof(fieldValues[NodeFields::zFar]))
    {
        zoom = std::stof(fieldValues[NodeFields::zoom]);
        transform.pivot = Utils::deserializeVec2(fieldValues[NodeFields::pivot]);
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
        ss << Utils::createKeyValuePair(NodeFields::type, typeName) << std::endl;
        ss << Utils::createKeyValuePair(NodeFields::name, name) << std::endl;
        ss << Utils::createKeyValuePair(NodeFields::position, Utils::serializeVec2(getPosition())) << std::endl;
        ss << Utils::createKeyValuePair(NodeFields::rotation, std::to_string(getRotation())) << std::endl;
        ss << Utils::createKeyValuePair(NodeFields::scale, Utils::serializeVec2(getScale())) << std::endl;
        ss << Utils::createKeyValuePair(NodeFields::pivot, Utils::serializeVec2(getPivot())) << std::endl;
        ss << Utils::createKeyValuePair(NodeFields::zNear, std::to_string(zNear)) << std::endl;
        ss << Utils::createKeyValuePair(NodeFields::zFar, std::to_string(zFar)) << std::endl;
        ss << Utils::createKeyValuePair(NodeFields::zoom, std::to_string(zoom)) << std::endl;
        ss << NodeFields::childrenStart << std::endl;
        for (Node* child : children)
            ss << child->serialize() << std::endl;
        ss << NodeFields::childrenEnd << std::endl;
        return ss.str();
    }
    
    void Camera2D::renderFields() 
    {
        Node2D::renderFields();

        ImGui::DragFloat("Z-Near", &zNear, 0.01f);
        ImGui::DragFloat("Z-Far", &zFar, 0.01f);
        ImGui::DragFloat("Zoom", &zoom, 0.01f);
    }

}