#include "node.hpp"
#include "prim_exception.hpp"
#include <algorithm>
#include "renderer.hpp"
#include "node_utils.hpp"
#include "imgui.h"

namespace prim
{

    Node::Node(std::string name) : name(name), nodePath(this)
    {
    }

    Node::Node(FieldValues& fieldValues)
        : name(fieldValues[StateFields::name]), nodePath(this)
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
        node->parent = this;
        children.push_back(node);
        node->updateNodePath();
    }

    void Node::removeChild(Node* node)
    {
        auto foundChild = std::find_if(children.begin(), children.end(), [node](const Node* child) -> bool { return child->id == node->id;});
        if (foundChild == children.end()) throw PRIM_EXCEPTION("Couldn't find node '" + node->name + "' among the children.");
        (*foundChild)->parent = nullptr;
        children.erase(foundChild);
        node->updateNodePath();
    }

    void Node::orphanize()
    {
        if (parent) parent->removeChild(this);
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

    void Node::renderFields()
    {
        static const unsigned int maxBufferSize = 200u;
        static char nameBuffer[maxBufferSize];
        std::copy(name.begin(), name.end(), nameBuffer);
        if (ImGui::InputText("Name", nameBuffer, maxBufferSize))
            setName(nameBuffer);
    }

}