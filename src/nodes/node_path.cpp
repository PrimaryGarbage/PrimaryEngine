#include "node_path.hpp"
#include "utils.hpp"
#include "node.hpp"
#include <sstream>

namespace prim
{
    NodePath::NodePath(const Node* node)
    {
        setPath(node);
    }

    NodePath::NodePath(std::string nodePath) : pathList(parsePath(nodePath)), pathString(nodePath)
    {
    }

    bool NodePath::operator==(const NodePath other) const
    {
        return string() == other.string();
    }

    std::list<std::string> NodePath::parsePath(std::string path)
    {
        std::vector<std::string> nodeNames = Utils::splitString(path, pathDelimiter);
        return std::list<std::string>(nodeNames.begin(), nodeNames.end());
    }

    NodePath::NodePath(std::list<std::string>&& pathList) : pathList(std::move(pathList))
    {
    }

    std::string NodePath::string() const
    {
        if (pathString.empty())
        {
            std::stringstream ss;
            for (const std::string nodeName : pathList)
            {
                ss << pathDelimiter << nodeName;
            }
            pathString = std::move(ss.str());
        }

        return pathString;
    }

    void NodePath::setPath(const Node* node)
    {
        pathList.clear();
        if (node->getParent()) pathList = node->getNodePath().pathList;
        pathList.push_back(node->getName());
        pathString = string();
    }

    std::string NodePath::front() const
    {
        return pathList.front();
    }

    std::string NodePath::back() const
    {
        return pathList.back();
    }

    NodePath NodePath::pop_back() const
    {
        return NodePath(std::list<std::string>(pathList.begin(), --pathList.end()));
    }

    NodePath NodePath::pop_front() const
    {
        return NodePath(std::list<std::string>(++pathList.begin(), pathList.end()));
    }
}