#include "node_base.hpp"
#include <exception>
#include "prim_exception.hpp"

namespace prim
{

    unsigned int NodeBase::getUniqueId()
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

    unsigned int NodeBase::freeUniqueId(unsigned int id)
    {
        idPool[id] = false;
        freeIds.push(id);
    }

    NodeBase::NodeBase() : id(getUniqueId())
    {

    }

    NodeBase::~NodeBase()
    {
        freeUniqueId(id);
    }

}
