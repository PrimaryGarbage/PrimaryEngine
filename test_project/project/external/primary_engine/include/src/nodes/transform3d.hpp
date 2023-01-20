#ifndef __TRANSFORM3D_HPP__
#define __TRANSFORM3D_HPP__

#include "glm/glm.hpp"

namespace prim
{
    struct Transform3D
    {
        glm::vec3 position { 0.0f, 0.0f, 0.0f};
        glm::vec3 rotation { 0.0f, 0.0f, 0.0f};
        glm::vec3 scale { 1.0f, 1.0f, 1.0f};
        glm::vec3 pivot { 0.0f, 0.0f, 0.0f};
    };

}

#endif // __TRANSFORM3D_HPP__