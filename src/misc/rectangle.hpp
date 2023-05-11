#ifndef __RECTANGLE_HPP__
#define __RECTANGLE_HPP__

#include "GLM/glm.hpp"
#include "shape2d.hpp"

namespace prim
{
    class Rectangle final : public Shape2D
    {
    private:
        // internal values
        glm::vec2 llPoint;
        glm::vec2 ruPoint;

        // primary values
        glm::vec2 position;
        glm::vec2 size;
        glm::vec2 pivot;
        float rotation;

        void calculateInternalValues();
    public:

        Rectangle() = default;
        Rectangle(glm::vec2 position, glm::vec2 size, glm::vec2 pivot, float rotation);

        float getArea() const override;
        bool inside(glm::vec2 point) const override;
        bool inside(float x, float y) const override;
        inline glm::vec2 getPosition() const { return position; }
        inline glm::vec2 getSize() const { return size; }
        inline float getRotation() const { return rotation; }
        inline glm::vec2 getPivot() const { return pivot; }
        inline void setPosition(glm::vec2 position) { this->position = position; calculateInternalValues(); }
        inline void setSize(glm::vec2 size) { this->size = size; calculateInternalValues(); }
        inline void setRotation(float rotation) { this->rotation = rotation; calculateInternalValues(); }
        inline void setPivot(glm::vec2 pivot) { this->pivot = pivot; calculateInternalValues(); }
    };
}

#endif // __RECTANGLE_HPP__