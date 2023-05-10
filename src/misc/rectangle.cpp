#include "rectangle.hpp"
#include "GLM/gtx/rotate_vector.hpp"

namespace prim
{
    Rectangle::Rectangle(glm::vec2 position, glm::vec2 size, glm::vec2 pivot, float rotation) 
        :
        position(position), size(size), pivot(pivot), rotation(rotation)
    {
        calculateInternalValues();
    }

    float Rectangle::getArea() const
    {
        return size.x * size.y;
    }
    
    bool Rectangle::inside(glm::vec2 point) const
    {
        // rotate point around pivot
        glm::vec2 pivotToPoint = point - position;
        point = glm::rotate(pivotToPoint, -rotation) + position;

        return point.x > llPoint.x && point.x < ruPoint.x
            && point.y > llPoint.y && point.y < ruPoint.y;
    }
    
    bool Rectangle::inside(float x, float y) const
    {
        return inside(glm::vec2(x, y));
    }

    void Rectangle::calculateInternalValues()
    {
        llPoint = position - size * pivot;
        ruPoint = llPoint + size;
    }
}