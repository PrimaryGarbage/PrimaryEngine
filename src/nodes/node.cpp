#include "node.hpp"
#include "prim_exception.hpp"
#include <algorithm>
#include "../graphics/renderer.hpp"
#include "imgui.h"
#include <sstream>
#include "utils.hpp"
#include "node_utils.hpp"

namespace prim
{
    Node::Node() : Node(generateNodeName(this))
    {
    }

    Node::Node(std::string name) : name(name), nodePath(this)
    {
    }

    Node::~Node()
    {
    }

    void Node::start()
    {
        startChildren();
    }

    void Node::update(float deltaTime)
    {
        updateChildren(deltaTime);
    }

    void Node::draw(Renderer& renderer)
    {
        drawChildren(renderer);
    }

    void Node::updateNodePath()
    {
        nodePath.setPath(this);
        for (Node* child : children) child->updateNodePath();
    }
    
    void Node::addChild(Node* node)
    {
        if (node->parent) throw PRIM_EXCEPTION("Can't add node '" + node->name + "' to children. Remove this node from it's parent first.");
        if (std::find(children.begin(), children.end(), node) != children.end()) throw PRIM_EXCEPTION("Node '" + node->name + "was already added.");
        node->parent = this;
        children.push_back(node);
        node->updateNodePath();
    }
    
    void Node::insertAfter(Node* node) 
    {
        if (node->parent) throw PRIM_EXCEPTION("Can't insert node '" + node->name + "'. Remove this node from it's parent first.");
        node->parent = this->parent;
        node->parent->children.insert(std::find(parent->children.begin(), parent->children.end(), this) + 1, node);
    }
    
    void Node::addSibling(Node* node) 
    {
        if(!parent) throw PRIM_EXCEPTION("Can't add sibling: Node has no parent");
        parent->addChild(node);
    }

    void Node::insertBefore(Node* node) 
    {
        if (node->parent) throw PRIM_EXCEPTION("Can't insert node '" + node->name + "'. Remove this node from it's parent first.");
        node->parent = this->parent;
        node->parent->children.insert(std::find(parent->children.begin(), parent->children.end(), this), node);
    }

    void Node::removeChild(Node* node)
    {
        auto foundChild = std::find(children.begin(), children.end(), node);
        if (foundChild == children.end()) throw PRIM_EXCEPTION("Couldn't find node '" + node->name + "' among the children.");
        (*foundChild)->parent = nullptr;
        children.erase(foundChild);
        node->updateNodePath();
    }
    
    bool Node::hasChild(Node* node) 
    {
        return std::find(children.begin(), children.end(), node) != children.end();
    }

    void Node::orphanize()
    {
        if (parent)
        {
            if(parent->hasChild(this))
                parent->removeChild(this);
            else
                parent = nullptr;
        }
    }
    
    void Node::startChildren() 
    {
        for(Node* child : children) child->start();
    }
    
    void Node::drawChildren(Renderer& renderer) 
    {
        for(Node* child : children) child->draw(renderer);
    }
    
    void Node::updateChildren(float deltaTime) 
    {
        for(Node* child : children) child->update(deltaTime);
    }

    const std::vector<Node*>& Node::getChildren() const
    {
        return children;
    }

    const Node* Node::getParent() const
    {
        return parent;
    }

    std::string Node::serialize(bool withChildren) const
    {
        std::stringstream ss;
        ss << Utils::createKeyValuePair(StateFields::type, type()) << std::endl;
        ss << Utils::createKeyValuePair(StateFields::name, name) << std::endl;

        if (withChildren) ss << serializeChildren();

        return ss.str();
    }

    std::string Node::serializeChildren() const
    {
        std::stringstream ss;
        ss << StateFields::childrenStart << std::endl;
        for (Node* child : children)
            ss << child->serialize() << std::endl;
        ss << StateFields::childrenEnd << std::endl;
        return ss.str();
    }
    
    void Node::deserialize(FieldValues& fieldValues) 
    {
        setName(fieldValues[StateFields::name]);
    }

    NodePath Node::getNodePath() const
    {
        return nodePath;
    }

    std::string Node::getName() const
    {
        return name;
    }

    void Node::setName(std::string name)
    {
        this->name = name;
        updateNodePath();
    }

    Node* Node::findChild(std::string name) const
    {
        Node* child = nullptr;
        for (Node* child : children)
        {
            if (child->name == name)
                return child;

            child = child->findChild(name);
            if (child) return child;
        }

        return child;
    }

    void Node::renderFields(SceneEditor* sceneEditor)
    {
        static const unsigned int maxBufferSize = 100u;
        static const ImVec4 typeTextColor = { 0.1f, 0.9f, 0.1f, 1.0f };
        static char nameBuffer[maxBufferSize];
        std::copy(name.begin(), name.end(), nameBuffer);
        std::fill(&nameBuffer[name.length()], &nameBuffer[maxBufferSize - 1], 0);
        ImGui::TextColored(typeTextColor, type());
        if (ImGui::InputText("Name", nameBuffer, maxBufferSize))
        {
            setName(nameBuffer);
        }
    }
    
    void Node::unbind() 
    {
        if(cloneBound)
        {
            cloneBound = false;
        }
    }

    glm::vec2 Node::getPosition() const
    { return glm::vec2(); }
    
    float Node::getRotation() const
    { return 0.0f; }
    
    glm::vec2 Node::getScale() const
    { return glm::vec2(1.0f, 1.0f); }
    
    glm::vec2 Node::getPivot() const
    { return glm::vec2(); }
    
    glm::vec2 Node::getGlobalPosition() const
    { return glm::vec2(); }
    
    float Node::getGlobalRotation() const
    { return 0.0f; }
    
    glm::vec2 Node::getGlobalScale() const
    { return glm::vec2(1.0f, 1.0f); }
    
    glm::vec2 Node::forward() const
    { return glm::vec2(0.0f, 1.0f); }
    
    glm::vec2 Node::backward() const
    { return glm::vec2(0.0f, -1.0f); }
    
    glm::vec2 Node::left() const
    { return glm::vec2(-1.0f, 0.0f); }
    
    glm::vec2 Node::right() const
    { return glm::vec2(1.0f, 0.0f); }
    
    void Node::setPosition(glm::vec2 v) 
    {}
    
    void Node::setRotation(float angle) 
    {}
    
    void Node::setScale(float s) 
    {}
    
    void Node::setScale(glm::vec2 s) 
    {}
    
    void Node::setPivot(glm::vec2 pivot) 
    {}
    
    void Node::setGlobalPosition(glm::vec2 v) 
    {}
    
    void Node::setGlobalRotation(float angle) 
    {}
    
    void Node::setGlobalScale(float s) 
    {}
    
    void Node::setGlobalScale(glm::vec2 s) 
    {}

}