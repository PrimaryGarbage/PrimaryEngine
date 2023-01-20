#include "rectangle.hpp"

namespace prim
{
    float Rectangle::getArea() const
    {
        return getWidth() * getHeight();
    }
    
    bool Rectangle::inside(glm::vec2 point) const
    {
        return point.x > luPoint.x && point.x < rlPoint.y
            && point.y > rlPoint.y && point.y < luPoint.y;
    }
    
    bool Rectangle::inside(float x, float y) const
    {
        return x > luPoint.x && x < rlPoint.y
            && y > rlPoint.y && y < luPoint.y;
    }
    
    float Rectangle::getWidth() const
    {
        return rlPoint.x - luPoint.x;   
    }
    
    float Rectangle::getHeight() const
    {
        return luPoint.y - rlPoint.y;
    }
    
    glm::vec2 Rectangle::getSize() const
    {
        return glm::vec2(getWidth(), getHeight());
    }
}