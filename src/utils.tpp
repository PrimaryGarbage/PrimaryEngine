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
}


#endif // __UTILS_TEMPLATE_TPP__