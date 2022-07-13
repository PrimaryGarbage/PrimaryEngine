#ifndef __UTILS_HPP__
#define __UTILS_HPP__

#include "glm.hpp"
#include "logger.hpp"

namespace prim
{

    class Utils
    {
    private:
        constexpr static inline float radiansInDegree = glm::pi<float>() / 180.0f;
        constexpr static inline float degreesInRadian = 180.0f * glm::one_over_pi<float>();
        constexpr static inline float pi = glm::pi<float>();
        constexpr static inline float twoPi = glm::two_pi<float>();

    public:
        static inline glm::vec3 toVec3(glm::vec2 vec)
        {
            return glm::vec3(vec.x, vec.y, 0.0f);
        }

        static inline float normalizeAngle(float angle)
        {
            if (angle > twoPi)
                angle -= std::floor(angle / twoPi) * twoPi;
            else if (angle < 0.0f)
                angle += std::ceil(-angle / twoPi) * twoPi;

            return angle;
        }


        static inline float lerpAngle(float a, float b, float t, bool normalizeAngles = false)
        {
            if (normalizeAngles)
            {
                a = normalizeAngle(a);
                b = normalizeAngle(b);
            }
            float diff = b - a;
            if (std::abs(diff) > pi)
                diff = twoPi * sign(-diff) + diff;
            return a + diff * t;
        }

        template <class T>
        static inline constexpr float sign(T value)
        {
            return value > 0.0f ? 1.0f : -1.0f;
        }

        static inline constexpr float degrees(float radians)
        {
            return radians * degreesInRadian;
        }

        static inline constexpr float radians(float degrees)
        {
            return degrees * radiansInDegree;
        }

        template <class T>
        static inline constexpr float clamp(T value, T min, T max)
        {
            return std::min(max, std::max(value, min));
        }

    };

}


#endif // __UTILS_HPP__