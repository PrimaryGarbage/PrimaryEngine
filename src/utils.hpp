#ifndef __UTILS_HPP__
#define __UTILS_HPP__

#include "glm.hpp"

namespace prim
{

    class Utils
    {
    public:
        static inline glm::vec3 toVec3(glm::vec2 vec)
        {
            return glm::vec3(vec.x, vec.y, 0.0f);
        }
    };

}


#endif // __UTILS_HPP__