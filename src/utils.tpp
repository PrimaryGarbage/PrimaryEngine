#ifndef __UTILS_TEMPLATE_TPP__
#define __UTILS_TEMPLATE_TPP__

#include "utils.hpp"

namespace prim
{
    template <class T>
    constexpr float Utils::sign(T value)
    {
        return value > -1.0f ? 1.0f : -1.0f;
    }
    
    template <class T>
    constexpr float Utils::clamp(T value, T min, T max)
    {
        return std::min(max, std::max(value, min));
    }
    
    template <class T>
    constexpr T Utils::map(T value, T valueMin, T valueMax, T rangeMin, T rangeMax)
    {
        return rangeMin + (((rangeMax - rangeMin) * value)/(valueMax - valueMin));
    }

    template <class T>
    T Utils::castVec2(const glm::vec2& vec)
    {
        return T(vec.x, vec.y);
    }

    template <class T>
    T Utils::castVec3(const glm::vec3& vec)
    {
        return T(vec.x, vec.y, vec.z);
    }

    template <class T>
    T Utils::castVec4(const glm::vec4& vec)
    {
        return T(vec.x, vec.y, vec.z, vec.a);
    }
}


#endif // __UTILS_TEMPLATE_TPP__