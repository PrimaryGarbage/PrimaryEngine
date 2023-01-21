#ifndef __RECTANGLE_HPP__
#define __RECTANGLE_HPP__

#include "glm/glm.hpp"
#include "shape2d.hpp"

namespace prim
{
    struct Rectangle final : public Shape2D
    {
        glm::vec2 luPoint;
        glm::vec2 rlPoint;

        float getArea() const override;
        bool inside(glm::vec2 point) const override;
        bool inside(float x, float y) const override;
        float getWidth() const;
        float getHeight() const;
        glm::vec2 getSize() const;
    };
}

#endif // __RECTANGLE_HPP__