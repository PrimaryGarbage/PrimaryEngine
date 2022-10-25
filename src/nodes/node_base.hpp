#ifndef __NODE_BASE_HPP__
#define __NODE_BASE_HPP__

#include <stack>
#include <string>
#include "typedefs.hpp"

namespace prim
{
    class NodeBase
    {
    private:
        static constexpr unsigned int maxId = 100000;
        inline static bool idPool[maxId]{};
        inline static std::stack<unsigned int> freeIds;
    protected:
        const unsigned int id;

        static unsigned int getUniqueId();
        void freeUniqueId(unsigned int id);


    public:
        NodeBase();
        NodeBase(const NodeBase& other);
        virtual ~NodeBase();

        inline uint getId() const { return id; }

        virtual std::string serialize(bool withChildren = true) const = 0;
        virtual std::string serializeChildren() const = 0;
        virtual void deserialize(FieldValues& fieldValues) = 0;
    };


}

#endif // __NODE_BASE_HPP__