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
        point = glm::rotate(point - pivotPosition, -rotation) + pivotPosition;

        return point.x > llPoint.x && point.x < ruPoint.x
            && point.y > llPoint.y && point.y < ruPoint.y;
    }
    
    bool Rectangle::inside(float x, float y) const
    {
        // rotate point around pivot
        glm::vec2 point = glm::rotate(glm::vec2(x, y) - pivotPosition, -rotation) + pivotPosition;

        return point.x > llPoint.x && point.x < ruPoint.x
            && point.y > llPoint.y && point.y < ruPoint.y;
    }

    void Rectangle::calculateInternalValues()
    {
        // vector from rect position to pivot point
        pivotOffset = size * pivot;
        // pivot vector from the coordinate center
        pivotPosition = position + pivotOffset;
        llPoint = glm::rotate(-pivotOffset, -rotation) + pivotPosition;
        ruPoint = glm::rotate(position + size - pivotPosition, -rotation) + pivotPosition;
    }
}