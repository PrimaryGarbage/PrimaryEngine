#include "camera2d.hpp"
#include "graphics/renderer.hpp"
#include "GLM/gtc/matrix_transform.hpp"
#include "utils.hpp"
#include "imgui.h"
#include "node_utils.hpp"

namespace prim
{
    Camera2D::Camera2D(): Camera2D(generateNodeName(this))
    {
    }

    Camera2D::Camera2D(std::string name)
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

    std::string Camera2D::serialize(bool withChildren) const
    {
        std::stringstream ss;

        ss << Node2D::serialize(false);

        ss << Utils::createKeyValuePair(StateValues::zNear, std::to_string(zNear)) << std::endl;
        ss << Utils::createKeyValuePair(StateValues::zFar, std::to_string(zFar)) << std::endl;
        ss << Utils::createKeyValuePair(StateValues::zoom, std::to_string(zoom)) << std::endl;

        if(withChildren) ss << serializeChildren();

        return ss.str();
    }
    
    void Camera2D::restore(NodeValues& nodeValues) 
    {
        Node2D::restore(nodeValues);

        zNear = std::stof(nodeValues[StateValues::zNear]);
        zFar = std::stof(nodeValues[StateValues::zFar]);
        zoom = std::stof(nodeValues[StateValues::zoom]);
        transform.pivot = Utils::deserializeVec2(nodeValues[StateValues::pivot]);
    }
    
    void Camera2D::renderFields(SceneEditor* sceneEditor) 
    {
        Node2D::renderFields(sceneEditor);

        static bool current;

        current = isCurrent();

        ImGui::DragFloat("Z-Near", &zNear, 0.01f);
        ImGui::DragFloat("Z-Far", &zFar, 0.01f);
        ImGui::DragFloat("Zoom", &zoom, 0.01f);
        if(ImGui::Checkbox("Current", &current))
        {
            renderer->setCurrentCamera(current ? this : nullptr);
        }
    }
    
    glm::vec2 Camera2D::worldToScreen(glm::vec3 point)
    {
        glm::mat4 viewMat = calculateViewMatrix();
        glm::vec4 result = viewMat * glm::vec4(point.x, point.y, point.z, 1.0f);
        return glm::vec2(result.x, result.y);
    }
    
    glm::vec3 Camera2D::screenToWorld(glm::vec2 point)
    {
        glm::mat4 viewMat = calculateViewMatrix();
        glm::mat4 viewMatInv = glm::inverse(viewMat);
        glm::vec4 result = viewMatInv * glm::vec4(point.x, point.y, 0.0f, 1.0f);
        return glm::vec3(result.x, result.y, result.z);
    }

}