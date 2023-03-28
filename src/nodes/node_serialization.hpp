#ifndef __NODE_SERIALIZATION_HPP__
#define __NODE_SERIALIZATION_HPP__

namespace prim
{
    class NodeSerialization
    {
    public:
        struct Symbols
        {
            inline static const char* header = "Node";
            inline static const char* type = "type";
            inline static const char* childrenStart = "children_start";
            inline static const char* childrenEnd = "children_end";
        };
    };
}

#endif // __NODE_SERIALIZATION_HPP__