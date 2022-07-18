#ifndef __NODE_SERIALIZER_HPP__
#define __NODE_SERIALIZER_HPP__

#include <string>
#include <sstream>
#include "node.hpp"
#include "node2d.hpp"
#include "node_utils.hpp"

namespace prim
{
    struct NodeFields 
    {
        inline static const char* type = "type";
        inline static const char* name = "name";
        inline static const char* position = "position";
        inline static const char* rotation = "rotation";
        inline static const char* scale = "scale";
        inline static const char* pivot = "pivot";
    };

    class NodeSerializer
    {
    private:
        inline static const char keyValueSeparator = '=';
    public:
        template<class T>
        inline static std::string serialize(T* node)
        {
            return "unknown node type";
        }

        template<>
        inline static std::string serialize<Node>(Node* node)
        {
            std::stringstream ss;
            ss << createKeyValuePair(NodeFields::type, getNodeTypeName<Node>()) << std::endl;
            ss << createKeyValuePair(NodeFields::name, node->name) << std::endl;
            ss << '{\n';
            for (Node* child : node->getChildren())
                ss << serialize(child) << std::endl;
            ss << '}\n';
            return ss.str();
        }

        template<>
        inline static std::string serialize<Node2D>(Node2D* node)
        {
            std::stringstream ss;
            ss << createKeyValuePair(NodeFields::type, getNodeTypeName<Node>()) << std::endl;
            ss << createKeyValuePair(NodeFields::name, node->name) << std::endl;
            ss << createKeyValuePair(NodeFields::position, serializeVec2(node->getPosition())) << std::endl;
            ss << createKeyValuePair(NodeFields::rotation, std::to_string(node->getRotation())) << std::endl;
            ss << createKeyValuePair(NodeFields::scale, serializeVec2(node->getScale())) << std::endl;
            ss << createKeyValuePair(NodeFields::pivot, serializeVec2(node->getPivot())) << std::endl;
            ss << '{\n';
            for (Node* child : node->getChildren())
                ss << serialize(child) << std::endl;
            ss << '}\n';
            return ss.str();
        }

        Node* deserialize(std::string data)
        {
            
        }

        static inline std::string createKeyValuePair(std::string key, std::string value)
        {
            return std::string(std::move(key) + keyValueSeparator + std::move(value));
        }

        static inline std::string serializeVec2(const glm::vec2& vec)
        {
            return std::to_string(vec.x) + ',' + std::to_string(vec.y);
        }
    };
}

#endif // __NODE_SERIALIZER_HPP__