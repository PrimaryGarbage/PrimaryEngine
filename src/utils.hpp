#ifndef __UTILS_HPP__
#define __UTILS_HPP__

#include "glm.hpp"
#include "gtc/constants.hpp"
#include "logger.hpp"
#include <vector>
#include <unordered_map>
#include "imgui.h"
#include <sstream>

namespace prim
{

    class Utils
    {
    private:
        constexpr static inline float radiansInDegree = glm::pi<float>() / 180.0f;
        constexpr static inline float degreesInRadian = 180.0f * glm::one_over_pi<float>();
        constexpr static inline float pi = glm::pi<float>();
        constexpr static inline float twoPi = glm::two_pi<float>();
        constexpr static inline char keyValueSeparator = '=';
        constexpr static inline char vecSeparator = ',';

    public:

        class Color
        {
        public:
            static inline constexpr ImVec4 White = { 1.0f, 1.0f, 1.0f, 1.0f };
            static inline constexpr ImVec4 Red = { 1.0f, 0.0f, 0.0f, 1.0f };
            static inline constexpr ImVec4 Green = { 0.0f, 1.0f, 0.0f, 1.0f };
            static inline constexpr ImVec4 Blue = { 0.0f, 0.0f, 1.0f, 1.0f };
            static inline constexpr ImVec4 Black = { 0.0f, 0.0f, 0.0f, 1.0f };
        };


        static inline glm::vec3 toVec3(glm::vec2 vec, float z = 0.0f)
        {
            return glm::vec3(vec.x, vec.y, z);
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

        static std::vector<std::string> splitString(std::string str, std::string delimiter)
        {
            std::vector<std::string> strings;
            size_t pos = 0, prevPos = 0;
            while ((pos = str.find(delimiter)) != std::string::npos)
            {
                strings.emplace_back(std::move(str.substr(prevPos, pos)));
                prevPos = pos;
            }
            strings.push_back(std::move(str.substr(pos)));

            return strings;
        }

        static inline std::vector<std::string> splitString(const std::string& str, const char delimiter)
        {
            std::vector<std::string> strings;
            int pos = -1, prevPos = -1;
            while ((pos = str.find(delimiter, pos + 1)) != std::string::npos)
            {
                std::string substr = str.substr(prevPos, pos - ++prevPos);
                if (!substr.empty()) strings.emplace_back(std::move(substr));
                prevPos = pos;
            }
            strings.push_back(std::move(str.substr(++prevPos)));

            return strings;
        }

        inline static std::string createKeyValuePair(std::string key, std::string value)
        {
            return std::string(std::move(key) + keyValueSeparator + std::move(value));
        }

        inline static std::pair<std::string, std::string> parseKeyValuePair(const std::string& line)
        {
            size_t pos = line.find(keyValueSeparator);
            std::pair<std::string, std::string> result(line.substr(0, pos), line.substr(pos + 1));
            return std::pair(line.substr(0, pos), line.substr(pos + 1));
        }

        inline static std::string serializeVec2(const glm::vec2& vec)
        {
            return std::to_string(vec.x) + vecSeparator + std::to_string(vec.y);
        }

        inline static glm::vec2 deserializeVec2(const std::string& line)
        {
            std::vector<std::string> values = Utils::splitString(line, vecSeparator);
            return glm::vec2(std::stof(values[0]), std::stof(values[1]));
        }

        inline static std::string toLower(const std::string& str)
        {
            std::string lowerStr;
            lowerStr.reserve(str.size());
            for (const char& c : str) lowerStr.push_back(std::tolower(c));
            return lowerStr;
        }

        inline static std::string removeSceneFileExtension(const std::string& filename)
        {
            return filename.substr(0, filename.size() - 4);
        }
    };

}


#endif // __UTILS_HPP__