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

    Node::Node(std::unordered_map<std::string, std::string>& fieldValues)
        : name(fieldValues[NodeFields::name]), nodePath(this)
    {
    }

    Node::~Node()
    {
    }

    void Node::start()
    {
        START_CHILDREN
    }

    void Node::update(float deltaTime)
    {
        UPDATE_CHILDREN
    }

    void Node::draw(Renderer& renderer)
    {
        DRAW_CHILDREN
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

    const std::vector<Node*>& Node::getChildren() const
    {
        return children;
    }

    const Node* Node::getParent() const
    {
        return parent;
    }

    std::string Node::serialize() const
    {
        std::stringstream ss;
        ss << Utils::createKeyValuePair(NodeFields::type, typeName) << std::endl;
        ss << Utils::createKeyValuePair(NodeFields::name, name) << std::endl;
        ss << NodeFields::childrenStart << std::endl;
        for (Node* child : children)
            ss << child->serialize() << std::endl;
        ss << NodeFields::childrenEnd << std::endl;
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