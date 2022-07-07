#ifndef __TRANSFORM2D_HPP__
#define __TRANSFORM2D_HPP__

#include "glm.hpp"

namespace prim
{
    struct Transform2D
    {
        glm::vec2 position;
        float rotation;
        glm::vec2 scale;
    };

}

#endif // __TRANSFORM2D_HPP__