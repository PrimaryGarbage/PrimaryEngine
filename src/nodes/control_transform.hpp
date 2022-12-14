#ifndef __CONTROL_TRANSFORM_HPP__
#define __CONTROL_TRANSFORM_HPP__

#include "glm.hpp"

namespace prim
{
    struct ControlTransform
    {
        glm::vec2 position{ 0.0f, 0.0f };
        float rotation{ 0.0f };
        glm::vec2 size{ 1.0f, 1.0f };
        glm::vec2 pivot{ 0.0f, 0.0f };
        float zIndex = 0.0f;
    };
} // namespace prim


#endif // __CONTROL_TRANSFORM_HPP__