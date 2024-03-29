#ifndef __TRANSFORM2D_HPP__
#define __TRANSFORM2D_HPP__

#include "GLM/glm.hpp"

namespace prim
{
    struct Transform2D
    {
        glm::vec2 position { 0.0f, 0.0f };
        float rotation {0.0f};
        glm::vec2 scale { 1.0f, 1.0f };
        glm::vec2 pivot { 0.0f, 0.0f };
    };

}

#endif // __TRANSFORM2D_HPP__