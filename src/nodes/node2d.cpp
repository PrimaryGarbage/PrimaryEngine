#include "node2d.hpp"
#include "renderer.hpp"
#include "gtx/rotate_vector.hpp"
#include "gtx/vector_angle.hpp"
#include "utils.hpp"
#include "logger.hpp"
#include "node_utils.hpp"
#include "imgui.h"

namespace prim
{

    Node2D::Node2D(std::string name) : Node(name)
    {
    }
    
    Node2D::Node2D(std::unordered_map<std::string, std::string>& fieldValues) 
        : Node(fieldValues[NodeFields::name])
    {
        transform.position = Utils::deserializeVec2(fieldValues[NodeFields::position]);
        transform.rotation = std::stof(fieldValues[NodeFields::rotation]);
        transform.scale = Utils::deserializeVec2(fieldValues[NodeFields::scale]);
        transform.pivot = Utils::deserializeVec2(fieldValues[NodeFields::pivot]);
    }

    Node2D::~Node2D()
    {
    }

    void Node2D::start()
    {
        START_CHILDREN
    }

    void Node2D::update(float deltaTime)
    {
        UPDATE_CHILDREN
    }

    void Node2D::draw(Renderer& renderer)
    {
        DRAW_CHILDREN
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
        Node2D* parent2d = dynamic_cast<Node2D*>(parent);
        if (parent2d)
        {
            return parent2d->getGlobalPosition() + glm::rotate(transform.position, parent2d->getGlobalRotation());
        }
        else
        {
            return transform.position;
        }
    }

    float Node2D::getGlobalRotation() const
    {
        if (!parent) return transform.rotation;
        Node2D* parent2d = dynamic_cast<Node2D*>(parent);
        if (parent2d)
        {
            return parent2d->getGlobalRotation() + transform.rotation;
        }
        else
        {
            return transform.rotation;
        }
    }

    glm::vec2 Node2D::getGlobalScale() const
    {
        if (!parent) return transform.scale;
        Node2D* parent2d = dynamic_cast<Node2D*>(parent);
        if (parent2d)
        {
            return parent2d->getGlobalScale() * transform.scale;
        }
        else
        {
            return transform.scale;
        }
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
        Node2D* parent2d = dynamic_cast<Node2D*>(parent);
        if (parent2d)
        {
            transform.position = v - parent2d->getGlobalPosition();
        }
        else
        {
            transform.position = v;
        }
    }

    void Node2D::setGlobalRotation(float angle)
    {
        if (!parent) transform.rotation = Utils::normalizeAngle(angle);
        Node2D* parent2d = dynamic_cast<Node2D*>(parent);
        if (parent2d)
        {
            transform.rotation = Utils::normalizeAngle(angle - parent2d->getGlobalRotation());
        }
        else
        {
            transform.rotation = Utils::normalizeAngle(angle);
        }
    }

    void Node2D::setGlobalScale(glm::vec2 s)
    {
        if (!parent) transform.scale = s;
        Node2D* parent2d = dynamic_cast<Node2D*>(parent);
        if (parent2d)
        {
            transform.scale = s / parent2d->getGlobalScale();
        }
        else
        {
            transform.scale = s;
        }
    }

    std::string Node2D::serialize() const
    {
        std::stringstream ss;
        ss << Utils::createKeyValuePair(NodeFields::type, typeName) << std::endl;
        ss << Utils::createKeyValuePair(NodeFields::name, name) << std::endl;
        ss << Utils::createKeyValuePair(NodeFields::position, Utils::serializeVec2(getPosition())) << std::endl;
        ss << Utils::createKeyValuePair(NodeFields::rotation, std::to_string(getRotation())) << std::endl;
        ss << Utils::createKeyValuePair(NodeFields::scale, Utils::serializeVec2(getScale())) << std::endl;
        ss << Utils::createKeyValuePair(NodeFields::pivot, Utils::serializeVec2(getPivot())) << std::endl;
        ss << NodeFields::childrenStart << std::endl;
        for (Node* child : children)
            ss << child->serialize() << std::endl;
        ss << NodeFields::childrenEnd << std::endl;
        return ss.str();
    }
    
    void Node2D::visualizeOnUi() 
    {
        Node::visualizeOnUi();

        static float posBuffer[2];
        static float rotBuffer;
        static float scaleBuffer[2];
        static float pivotBuffer[2];

        posBuffer[0] = transform.position.x;
        posBuffer[1] = transform.position.y;

        rotBuffer = transform.rotation;

        scaleBuffer[0] = transform.scale.x;
        scaleBuffer[1] = transform.scale.y;

        pivotBuffer[0] = transform.pivot.x;
        pivotBuffer[1] = transform.pivot.y;

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
        if (ImGui::DragFloat2("Pivot", pivotBuffer, 0.01f))
        {
            setPivot(glm::vec2(pivotBuffer[0], pivotBuffer[1]));
        }
    }
}