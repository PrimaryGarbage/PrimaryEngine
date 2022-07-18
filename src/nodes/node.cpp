#include "node.hpp"
#include "prim_exception.hpp"
#include <algorithm>
#include <sstream>
#include "renderer.hpp"
#include "scene_manager.hpp"

namespace prim
{

    Node::Node(std::string name) : name(name)
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
    
    void Node::addChild(Node* node)
    {
        if(node->parent) throw PRIM_EXCEPTION("Can't add node '" + node->name + "' to children. Remove this node from it's parent first.");
        node->parent = this;
        children.push_back(node);
    }
    
    void Node::removeChild(Node* node)
    {
        auto foundChild = std::find_if(children.begin(), children.end(), [node](const Node* child) -> bool { return child->id == node->id;});
        if(foundChild == children.end()) throw PRIM_EXCEPTION("Couldn't find node '" + node->name + "' among the children.");
        (*foundChild)->parent = nullptr;
        children.erase(foundChild);
    }

    const std::vector<Node*>& Node::getChildren() const
    {
        return children;
    }

}