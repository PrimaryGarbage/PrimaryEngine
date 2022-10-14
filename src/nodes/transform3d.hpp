#ifndef __TRANSFORM3D_HPP__
#define __TRANSFORM3D_HPP__

#include "glm.hpp"

namespace prim
{
    struct Transform3D
    {
        glm::vec3 position;
        glm::vec3 rotation;
        glm::vec3 scale;
        glm::vec3 pivot;
    };

}

#endif // __TRANSFORM3D_HPP__