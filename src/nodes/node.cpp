#include "node.hpp"
#include "prim_exception.hpp"
#include "graphics/renderer.hpp"
#include "imgui.h"
#include "utils.hpp"
#include "node_utils.hpp"
#include "node_serialization.hpp"
#include <sstream>
#include <algorithm>

namespace prim
{
    Node::Node() : Node(generateNodeName(this))
    {
    }

    Node::Node(std::string name) : id(getUniqueId()), name(name), nodePath(this)
    {
    }

    Node::~Node()
    {
        // Logger::inst().logInfo("Deleting node: '" + this->name + "'");
        freeUniqueId(id);
        std::for_each(children.begin(), children.end(), [](Node* node) { delete node; });
    }

    unsigned int Node::getUniqueId()
    {
        if (!freeIds.empty())
        {
            int newId = freeIds.top();
            idPool[newId] = true;
            freeIds.pop();
            return newId;
        }
        else
        {
            for (unsigned int i = 0; i < maxId; ++i)
            {
                if (!idPool[i])
                {
                    idPool[i] = true;
                    return i;
                }
            }
        }

        throw PRIM_EXCEPTION("There are no more free node IDs");
    }

    Node* Node::createNode(std::string type) 
    {
        auto iter = nodeTypeMap.find(type);
        if(iter == nodeTypeMap.end()) throw PRIM_EXCEPTION("Couldn't find node type '" + type + "'");
        Node* newNode = iter->second();
        return newNode;
    }
    
    std::vector<std::string> Node::getAllNodeTypes() 
    {
        std::vector<std::string> result;
        result.reserve(nodeTypeMap.size());
        for(const auto& pair : nodeTypeMap)
            result.push_back(pair.first);
        return result;
    }

    void Node::freeUniqueId(unsigned int id)
    {
        idPool[id] = false;
        freeIds.push(id);
    }

    void Node::start()
    {
        NODE_START
    }

    void Node::uiUpdate(float deltaTime)
    {
        NODE_UI_UPDATE
    }

    void Node::update(float deltaTime)
    {
        NODE_UPDATE
    }

    void Node::lateUpdate(float deltaTime)
    {
        NODE_LATE_UPDATE
    }

    void Node::draw(Renderer& renderer)
    {
        NODE_DRAW
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
    
    void Node::addChildren(const std::vector<Node*>& children)
    {
        std::for_each(children.begin(), children.end(), [this](Node* child) { this->addChild(child); });
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
        ss << Utils::createKeyValuePair(StateValues::type, type()) << std::endl;
        ss << Utils::createKeyValuePair(StateValues::name, name) << std::endl;

        if (withChildren) ss << serializeChildren();

        return ss.str();
    }

    std::string Node::serializeChildren() const
    {
        std::stringstream ss;
        ss << StateValues::childrenStart << std::endl;
        for (Node* child : children)
            ss << child->serialize() << std::endl;
        ss << StateValues::childrenEnd << std::endl;
        return ss.str();
    }
    
    Node* Node::deserialize(const std::string& nodeStr) 
    {
        using Symbols = NodeSerialization::Symbols;

        Node* rootNode = nullptr;
        std::vector<std::string> lines = Utils::splitString(nodeStr, '\n');
        Utils::trimEmptyStrings(lines);
        std::stack<Node*> parentNodes;
        std::unordered_map<std::string, std::string> fields;

        for (const std::string& line : lines)
        {
            if (line.empty()) continue;

            if (line == Symbols::childrenStart)
            {
                Node* node = Node::createNode(fields[Symbols::type]);
                node->restore(fields);
                if (parentNodes.empty()) rootNode = node;
                else parentNodes.top()->addChild(node);
                parentNodes.push(node);
                fields.clear();
                continue;
            }

            if (line == Symbols::childrenEnd)
            {
                if (!fields.empty())
                {
                    Node* node = Node::createNode(fields[Symbols::type]);
                    node->restore(fields);
                    parentNodes.top()->addChild(node);
                }
                parentNodes.pop();
                continue;
            }

            std::pair<std::string, std::string> keyValuePair = Utils::parseKeyValuePair(line);
            fields.insert(keyValuePair);
        }

        return rootNode;
    }
    
    void Node::restore(NodeValues& nodeValues) 
    {
        setName(nodeValues[StateValues::name]);
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
    
    Node* Node::clone() const
    {
        std::string thisSerialized = serialize(true);   
        return Node::deserialize(thisSerialized);
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