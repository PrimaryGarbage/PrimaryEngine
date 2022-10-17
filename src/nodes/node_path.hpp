#ifndef __NODE_PATH_HPP__
#define __NODE_PATH_HPP__

#include <list>
#include <string>

namespace prim
{
    class Node;

    class NodePath
    {
    private:
        const static char pathDelimiter = '/';
        std::list<std::string> pathList;
        mutable std::string pathString;

        static std::list<std::string> parsePath(std::string path);
    public:
        NodePath() = default;
        NodePath(const Node* node);
        NodePath(std::string nodePath);
        NodePath(std::list<std::string>&& pathList);
        bool operator==(const NodePath other) const;

        std::string string() const;
        void setPath(const Node* node);
        std::string front() const;
        std::string back() const;
        NodePath pop_back() const;
        NodePath pop_front() const;
        inline bool empty() const { return pathList.empty(); }
    };
}

#endif // __NODE_PATH_HPP__