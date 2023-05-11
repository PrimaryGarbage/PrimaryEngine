#ifndef __NODE_TPP__
#define __NODE_TPP__

#include "node.hpp"

namespace prim
{
    template<class T>
    T* Node::findChild(std::string name) const
    {
        T* childT = nullptr;
        for (Node* child : children)
        {
            childT = dynamic_cast<T*>(child);
            if (childT)
            {
                if (!name.empty())
                    if (childT->name == name)
                        return childT;
                    else
                        childT = nullptr;
                else
                    return childT;
            }

            childT = child->findChild<T>(name);
            if (childT) return childT;
        }

        return childT;
    }
}

#endif // __NODE_TPP__