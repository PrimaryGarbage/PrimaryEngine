#ifndef __UTILS_HPP__
#define __UTILS_HPP__

#ifdef _WIN32
#define NOMINMAX
#include <windows.h>
#endif

#include "GLM/glm.hpp"
#include "GLM/gtc/constants.hpp"
#include "globals.hpp"

namespace prim
{

    class Utils
    {
    public:
        class Color
        {
        public:
            static inline constexpr glm::vec4 White = { 1.0f, 1.0f, 1.0f, 1.0f };
            static inline constexpr glm::vec4 Red = { 1.0f, 0.0f, 0.0f, 1.0f };
            static inline constexpr glm::vec4 Green = { 0.0f, 1.0f, 0.0f, 1.0f };
            static inline constexpr glm::vec4 Blue = { 0.0f, 0.0f, 1.0f, 1.0f };
            static inline constexpr glm::vec4 Black = { 0.0f, 0.0f, 0.0f, 1.0f };
        };

        constexpr static inline float radiansInDegree = glm::pi<float>() / 180.0f;
        constexpr static inline float degreesInRadian = 180.0f * glm::one_over_pi<float>();
        constexpr static inline float pi = glm::pi<float>();
        constexpr static inline float twoPi = glm::two_pi<float>();
        constexpr static inline char keyValueSeparator = '=';
        constexpr static inline char vecSeparator = ',';

        static glm::vec3 toVec3(glm::vec2 vec, float z = 0.0f);
        static glm::vec2 toVec2(glm::vec3 vec);
        static float normalizeAngle(float angle);
        static float lerpAngle(float a, float b, float t, bool normalizeAngles = false);
        static constexpr float degrees(float radians);
        static constexpr float radians(float degrees);
        static std::vector<std::string> splitString(std::string str, std::string delimiter);
        static std::vector<std::string> splitString(const std::string& str, const char delimiter);
        static void trimEmptyStrings(std::vector<std::string>& strings);
        static bool contains(const std::string& str, const std::string& substr);
        static bool startsWith(const std::string& source, const std::string& str);
        static std::string createKeyValuePair(std::string key, std::string value);
        static std::pair<std::string, std::string> parseKeyValuePair(const std::string& line);
        static std::string serializeVec2(const glm::vec2& vec);
        static std::string serializeVec3(const glm::vec3& vec);
        static std::string serializeVec4(const glm::vec4& vec);
        static glm::vec2 deserializeVec2(const std::string& line);
        static glm::vec3 deserializeVec3(const std::string& line);
        static glm::vec4 deserializeVec4(const std::string& line);
        static std::string toLower(const std::string& str);
        static fs::path getAppDirPath();
        static std::string currentDateTimeString(const char* format = "%d-%m-%Y %H:%M:%S");

        template <class T>
        static constexpr float sign(T value);
        template <class T>
        static constexpr float clamp(T value, T min, T max);
        template <class T>
        static constexpr T map(T value, T valueMin, T valueMax, T rangeMin, T rangeMax);
        template <class T> 
        static T castVec2(const glm::vec2& vec);
        template <class T> 
        static T castVec3(const glm::vec3& vec);
        template <class T> 
        static T castVec4(const glm::vec4& vec);
    };

}

#include "utils.tpp"


#endif // __UTILS_HPP__