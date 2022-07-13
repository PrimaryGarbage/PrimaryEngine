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

    public:
        static inline glm::vec3 toVec3(glm::vec2 vec)
        {
            return glm::vec3(vec.x, vec.y, 0.0f);
        }

        static inline float trimAngle(float angle)
        {
            if(std::abs(angle) < glm::two_pi<float>()) return angle;

            if (angle > 0)
                return angle - std::floor(angle / glm::two_pi<float>()) * glm::two_pi<float>();
            else
                return angle - std::ceil(angle / glm::two_pi<float>()) * glm::two_pi<float>();
        }

        static inline float lerpAngle(float a, float b, float t)
        {
            float diff = std::abs(b - a);
            Logger::printLine(std::to_string(diff));
            if (diff > glm::pi<float>())
               b += glm::two_pi<float>() * sign(b);
            return a + (b - a) * t;
            // const static glm::vec2 up(0.0f, 1.0f);
            // glm::vec2 v1 = glm::rotate(up, a);
            // glm::vec2 v2 = glm::rotate(up, b);
            // glm::vec2 result = glm::mix(v1, v2, t);
            // return glm::orientedAngle(result, up);
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
    };

}


#endif // __UTILS_HPP__