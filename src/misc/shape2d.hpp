#ifndef __SHAPE2D_HPP__
#define __SHAPE2D_HPP__

#include "GLM/glm.hpp"

namespace prim
{
    class Shape2D
    {
    public:
        virtual float getArea() const = 0;
        virtual bool inside(glm::vec2 point) const = 0;
        virtual bool inside(float x, float y) const = 0;
    };
}

#endif // __SHAPE2D_HPP__