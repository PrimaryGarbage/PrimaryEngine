#ifndef __NODE_BASE_HPP__
#define __NODE_BASE_HPP__

#include <stack>

namespace prim
{
    class NodeBase
    {
    private:
        static constexpr unsigned int maxId = 100000;
        inline static bool idPool[maxId] {};
        inline static std::stack<unsigned int> freeIds;
    protected:
        const unsigned int id;
        
        static unsigned int getUniqueId();
        void freeUniqueId(unsigned int id);

    public:
        NodeBase();
        virtual ~NodeBase();
    };
}

#endif // __NODE_BASE_HPP__