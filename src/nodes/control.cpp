#include "control.hpp"
#include "GLM/gtx/rotate_vector.hpp"
#include "GLM/gtx/vector_angle.hpp"
#include "camera_base.hpp"
#include "globals.hpp"
#include "imgui.h"
#include <sstream>

namespace prim
{

    Control::Control() : Node()
    {
    }
    
    Control::Control(std::string name) : Node(name)
    {
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
    
    glm::vec2 Control::getPosition() const 
    {
        return transform.position;
    }
    
    float Control::getRotation() const 
    {
        return transform.rotation;
    }
    
    glm::vec2 Control::getSize() const 
    {
        return transform.size;
    }
    
    glm::vec2 Control::getPivot() const 
    {
        return transform.pivot;
    }
    
    glm::vec2 Control::getAnchor() const
    {
        return transform.anchor;
    }
    
    glm::vec2 Control::getGlobalPosition() const 
    {
        if (!controlParent)
        {
            glm::vec2 windowSize = Globals::mainRenderer->getWindowSize();
            return transform.anchor * windowSize + transform.position;
        }
        return parent->getGlobalPosition() + glm::rotate(transform.position, parent->getGlobalRotation());
    }
    
    float Control::getGlobalRotation() const 
    {
        if (!controlParent)
            return transform.rotation;
        return parent->getGlobalRotation() + transform.rotation;
    }
    
    void Control::setPosition(glm::vec2 v) 
    {
        transform.position = v;
    }
    
    void Control::setRotation(float angle) 
    {
        transform.rotation = Utils::normalizeAngle(angle);
    }
    
    void Control::setSize(float width, float height) 
    {
        setSize(glm::vec2(width, height));
    }
    
    void Control::setSize(glm::vec2 s) 
    {
        transform.size = s;
    }
    
    void Control::setWidth(float width) 
    {
        transform.size.x = width;
    }
    
    void Control::setHeight(float height) 
    {
        transform.size.y = height;
    }
    
    void Control::setPivot(glm::vec2 pivot) 
    {
        transform.pivot = pivot;
    }
    
    void Control::setAnchor(glm::vec2 anchor) 
    {
        transform.anchor = anchor;
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
    
    void Control::addChild(Node* node) 
    {
        Node::addChild(node);
        Control* controlNode = dynamic_cast<Control*>(node);
        if(controlNode) controlNode->controlParent = this;
    }
    
    void Control::insertBefore(Node* node) 
    {
        Node::insertBefore(node);
        Control* controlNode = dynamic_cast<Control*>(node);
        if(controlNode) controlNode->controlParent = this->controlParent;
    }
    
    void Control::insertAfter(Node* node) 
    {
        Node::insertAfter(node);
        Control* controlNode = dynamic_cast<Control*>(node);
        if(controlNode) controlNode->controlParent = this->controlParent;
    }
    
    void Control::addSibling(Node* node) 
    {
        Node::addSibling(node);
        Control* controlNode = dynamic_cast<Control*>(node);
        if(controlNode) controlNode->controlParent = this->controlParent;
    }
    
    void Control::removeChild(Node* node) 
    {
        Node::removeChild(node);
        Control* controlNode = dynamic_cast<Control*>(node);
        if(controlNode) controlNode->controlParent = nullptr;
    }
    
    bool Control::hasChild(Node* node) 
    {
        return Node::hasChild(node);
    }
    
    void Control::orphanize() 
    {
        Node::orphanize();
        controlParent = nullptr;
    }
    
    std::string Control::serialize(bool withChildren) const 
    {
        std::stringstream ss;

        ss << Node::serialize(false);

        ss << Utils::createKeyValuePair(StateFields::position, Utils::serializeVec2(getPosition())) << std::endl;
        ss << Utils::createKeyValuePair(StateFields::rotation, std::to_string(getRotation())) << std::endl;
        ss << Utils::createKeyValuePair(StateFields::size, Utils::serializeVec2(getSize())) << std::endl;
        ss << Utils::createKeyValuePair(StateFields::pivot, Utils::serializeVec2(getPivot())) << std::endl;
        ss << Utils::createKeyValuePair(StateFields::anchor, Utils::serializeVec2(getAnchor())) << std::endl;

        if(withChildren) ss << serializeChildren();

        return ss.str();
    }
    
    void Control::restore(NodeValues& nodeValues) 
    {
        Node::restore(nodeValues);

        transform.position = Utils::deserializeVec2(nodeValues[StateFields::position]);
        transform.rotation = std::stof(nodeValues[StateFields::rotation]);
        transform.size = Utils::deserializeVec2(nodeValues[StateFields::size]);
        transform.pivot = Utils::deserializeVec2(nodeValues[StateFields::pivot]);
        transform.anchor = Utils::deserializeVec2(nodeValues[StateFields::anchor]);
    }
    
    void Control::renderFields(SceneEditor* sceneEditor) 
    {
        Node::renderFields(sceneEditor);

        ImGui::DragFloat2("Position", &transform.position.x);
        ImGui::DragFloat("Rotation", &transform.rotation, 0.01f);
        ImGui::DragFloat2("Size", &transform.size.x, 0.01f);
        ImGui::DragFloat("zIndex", &transform.zIndex, 0.01f);
        //ImGui::DragFloat2("Pivot", &transform.pivot.x, 0.01f);
        ImGui::DragFloat2("Anchor", &transform.anchor.x, 0.01f);
        
        glm::mat4 rendererViewMat = Globals::mainRenderer->getViewMat();
        Globals::mainRenderer->setViewMat(glm::mat4(1.0f));
        sceneEditor->drawSelectedNodePositionPoint(getGlobalPosition());
        Globals::mainRenderer->setViewMat(rendererViewMat);
    }

    
} // namespace prim
