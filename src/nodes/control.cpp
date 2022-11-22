#include "control.hpp"
#include "gtx/rotate_vector.hpp"
#include "gtx/vector_angle.hpp"

namespace prim
{

    Control::Control() : Node()
    {
    }
    
    Control::Control(std::string name) : Node(name)
    {
    }

    std::string Control::getPathToFontFile(std::string filename) 
    {
        return "./res/fonts/" + filename;      
    }

    void Control::move(glm::vec2 v) 
    {
        transform.position += v;
    }
    
    void Control::move(float x, float y) 
    {
        transform.position += glm::vec2(x, y);
    }
    
    void Control::rotate(float angle) 
    {
        transform.rotation += angle;
        transform.rotation = Utils::normalizeAngle(transform.rotation);
    }
    
    void Control::scale(float s) 
    {
        transform.scale *= glm::vec2(s);
    }
    
    void Control::scale(glm::vec2 s) 
    {
        
        transform.scale *= s;
    }
    
    glm::vec2 Control::getPosition() const 
    {
        return transform.position;
    }
    
    float Control::getRotation() const 
    {
        return transform.rotation;
    }
    
    glm::vec2 Control::getScale() const 
    {
        return transform.scale;
    }
    
    glm::vec2 Control::getPivot() const 
    {
        return transform.pivot;
    }
    
    glm::vec2 Control::getGlobalPosition() const 
    {
        if (!parent) return getPosition();
        return parent->getGlobalPosition() + glm::rotate(transform.position, parent->getGlobalRotation());
    }
    
    float Control::getGlobalRotation() const 
    {
        if (!parent) return transform.rotation;
        return parent->getGlobalRotation() + transform.rotation;
    }
    
    glm::vec2 Control::getGlobalScale() const 
    {
        if (!parent) return transform.scale;
        return parent->getGlobalScale() * transform.scale;
    }
    
    void Control::setPosition(glm::vec2 v) 
    {
        transform.position = v;
    }
    
    void Control::setRotation(float angle) 
    {
        transform.rotation = Utils::normalizeAngle(angle);
    }
    
    void Control::setScale(float s) 
    {
        transform.scale = glm::vec2(s);
    }
    
    void Control::setScale(glm::vec2 s) 
    {
        transform.scale = s;
    }
    
    void Control::setPivot(glm::vec2 pivot) 
    {
        transform.pivot = pivot;
    }
    
    void Control::setGlobalPosition(glm::vec2 v) 
    {
        if (!parent) transform.position = v;
        transform.position = v - parent->getGlobalPosition();
    }
    
    void Control::setGlobalRotation(float angle) 
    {
        if (!parent) transform.rotation = Utils::normalizeAngle(angle);
        transform.rotation = Utils::normalizeAngle(angle - parent->getGlobalRotation());
    }
    
    void Control::setGlobalScale(float s) 
    {
        if (!parent) transform.scale = glm::vec2(s, s);
        transform.scale = glm::vec2(s, s) / parent->getGlobalScale();
    }
    
    void Control::setGlobalScale(glm::vec2 s) 
    {
        if (!parent) transform.scale = s;
        transform.scale = s / parent->getGlobalScale();
    }
    
    std::string Control::serialize(bool withChildren) const 
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
    
    void Control::deserialize(FieldValues& fieldValues) 
    {
        Node::deserialize(fieldValues);

        transform.position = Utils::deserializeVec2(fieldValues[StateFields::position]);
        transform.rotation = std::stof(fieldValues[StateFields::rotation]);
        transform.scale = Utils::deserializeVec2(fieldValues[StateFields::scale]);
        transform.pivot = Utils::deserializeVec2(fieldValues[StateFields::pivot]);
    }
    
    void Control::renderFields() 
    {
        Node::renderFields();

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
    }

    
} // namespace prim
