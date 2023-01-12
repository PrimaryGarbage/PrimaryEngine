#include "node2d.hpp"
#include "../graphics/renderer.hpp"
#include "gtx/rotate_vector.hpp"
#include "gtx/vector_angle.hpp"
#include "utils.hpp"
#include "logger.hpp"
#include "imgui.h"
#include "node_utils.hpp"
#include "scene_editor.hpp"

namespace prim
{
    Node2D::~Node2D()
    {
    }

    void Node2D::start()
    {
        startChildren();
    }

    void Node2D::update(float deltaTime)
    {
        updateChildren(deltaTime);
    }

    void Node2D::draw(Renderer& renderer)
    {
        drawChildren(renderer);
    }

    void Node2D::move(glm::vec2 v)
    {
        transform.position += v;
    }

    void Node2D::move(float x, float y)
    {
        transform.position += glm::vec2(x, y);
    }

    void Node2D::rotate(float angle)
    {
        transform.rotation += angle;
        transform.rotation = Utils::normalizeAngle(transform.rotation);
    }

    void Node2D::scale(float s)
    {
        transform.scale *= glm::vec2(s);
    }

    void Node2D::scale(glm::vec2 s)
    {
        transform.scale *= s;
    }

    void Node2D::lookAt(glm::vec2 v)
    {
        const static glm::vec2 up(0.0f, 1.0f);
        setGlobalRotation(-glm::orientedAngle(glm::normalize(v - getGlobalPosition()), up));
    }

    void Node2D::lookAtSmooth(glm::vec2 v, float stiffness)
    {
        const static glm::vec2 up(0.0f, 1.0f);
        setGlobalRotation(Utils::lerpAngle(getGlobalRotation(), -glm::orientedAngle(glm::normalize(v - getGlobalPosition()), up), Utils::clamp(stiffness, 0.0f, 1.0f)));
    }

    glm::vec2 Node2D::getPosition() const
    {
        return transform.position;
    }

    float Node2D::getRotation() const
    {
        return transform.rotation;
    }

    glm::vec2 Node2D::getScale() const
    {
        return transform.scale;
    }

    glm::vec2 Node2D::getPivot() const
    {
        return transform.pivot;
    }

    glm::vec2 Node2D::getGlobalPosition() const
    {
        if (!parent) return getPosition();
        return parent->getGlobalPosition() + glm::rotate(transform.position, parent->getGlobalRotation());
    }

    float Node2D::getGlobalRotation() const
    {
        if (!parent) return transform.rotation;
        return parent->getGlobalRotation() + transform.rotation;
    }

    glm::vec2 Node2D::getGlobalScale() const
    {
        if (!parent) return transform.scale;
        return parent->getGlobalScale() * transform.scale;
    }

    glm::vec2 Node2D::forward() const
    {
        return glm::rotate(glm::vec2(0.0f, 1.0f), getGlobalRotation());
    }

    glm::vec2 Node2D::backward() const
    {
        return -forward();
    }

    glm::vec2 Node2D::left() const
    {
        return glm::rotate(glm::vec2(0.0f, 1.0f), getGlobalRotation() + glm::half_pi<float>());
    }

    glm::vec2 Node2D::right() const
    {
        return glm::rotate(glm::vec2(0.0f, 1.0f), getGlobalRotation() - glm::half_pi<float>());
    }

    void Node2D::setPosition(glm::vec2 v)
    {
        transform.position = v;
    }

    void Node2D::setRotation(float angle)
    {
        transform.rotation = Utils::normalizeAngle(angle);
    }

    void Node2D::setScale(float s)
    {
        transform.scale = glm::vec2(s);
    }

    void Node2D::setScale(glm::vec2 s)
    {
        transform.scale = s;
    }

    void Node2D::setPivot(glm::vec2 pivot)
    {
        transform.pivot = pivot;
    }

    void Node2D::setGlobalPosition(glm::vec2 v)
    {
        if (!parent) transform.position = v;
        transform.position = v - parent->getGlobalPosition();
    }

    void Node2D::setGlobalRotation(float angle)
    {
        if (!parent) transform.rotation = Utils::normalizeAngle(angle);
        transform.rotation = Utils::normalizeAngle(angle - parent->getGlobalRotation());
    }
    
    void Node2D::setGlobalScale(float s) 
    {
        if (!parent) transform.scale = glm::vec2(s, s);
        transform.scale = glm::vec2(s, s) / parent->getGlobalScale();
    }

    void Node2D::setGlobalScale(glm::vec2 s)
    {
        if (!parent) transform.scale = s;
        transform.scale = s / parent->getGlobalScale();
    }

    std::string Node2D::serialize(bool withChildren) const
    {
        std::stringstream ss;

        ss << Node::serialize(false);

        ss << Utils::createKeyValuePair(StateFields::position, Utils::serializeVec2(getPosition())) << std::endl;
        ss << Utils::createKeyValuePair(StateFields::rotation, std::to_string(getRotation())) << std::endl;
        ss << Utils::createKeyValuePair(StateFields::scale, Utils::serializeVec2(getScale())) << std::endl;
        ss << Utils::createKeyValuePair(StateFields::pivot, Utils::serializeVec2(getPivot())) << std::endl;

        if(withChildren) ss << serializeChildren();

        return ss.str();
    }
    
    void Node2D::deserialize(FieldValues& fieldValues) 
    {
        Node::deserialize(fieldValues);

        transform.position = Utils::deserializeVec2(fieldValues[StateFields::position]);
        transform.rotation = std::stof(fieldValues[StateFields::rotation]);
        transform.scale = Utils::deserializeVec2(fieldValues[StateFields::scale]);
        transform.pivot = Utils::deserializeVec2(fieldValues[StateFields::pivot]);
    }
    
    void Node2D::renderFields(SceneEditor* sceneEditor) 
    {
        Node::renderFields(sceneEditor);

        static float posBuffer[2];
        static float rotBuffer;
        static float scaleBuffer[2];

        posBuffer[0] = transform.position.x;
        posBuffer[1] = transform.position.y;

        rotBuffer = transform.rotation;

        scaleBuffer[0] = transform.scale.x;
        scaleBuffer[1] = transform.scale.y;

        if (ImGui::DragFloat2("Position", posBuffer))
        {
            setPosition(glm::vec2(posBuffer[0], posBuffer[1]));
        }
        if (ImGui::DragFloat("Rotation", &rotBuffer, 0.01f))
        {
            setRotation(rotBuffer);
        }
        if (ImGui::DragFloat2("Scale", scaleBuffer, 0.01f))
        {
            setScale(glm::vec2(scaleBuffer[0], scaleBuffer[1]));
        }

        sceneEditor->drawSelectedNodePositionPoint(getGlobalPosition());
    }
}