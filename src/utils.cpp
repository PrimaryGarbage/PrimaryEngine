#include "utils.hpp"

namespace prim
{
    glm::vec3 Utils::toVec3(glm::vec2 vec, float z)
    {
        return glm::vec3(vec.x, vec.y, z);
    }

    float Utils::normalizeAngle(float angle)
    {
        if (angle > twoPi)
            angle -= std::floor(angle / twoPi) * twoPi;
        else if (angle < 0.0f)
            angle += std::ceil(-angle / twoPi) * twoPi;

        return angle;
    }


    float Utils::lerpAngle(float a, float b, float t, bool normalizeAngles)
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

    constexpr float Utils::degrees(float radians)
    {
        return radians * degreesInRadian;
    }

    constexpr float Utils::radians(float degrees)
    {
        return degrees * radiansInDegree;
    }

    std::vector<std::string> Utils::splitString(std::string str, std::string delimiter)
    {
        std::vector<std::string> strings;
        size_t pos = 0, prevPos = 0;
        while ((pos = str.find(delimiter, pos + 1)) != std::string::npos)
        {
            strings.emplace_back(std::move(str.substr(prevPos, pos)));
            prevPos = pos;
        }
        strings.push_back(std::move(str.substr(prevPos + delimiter.length())));

        return strings;
    }

    std::vector<std::string> Utils::splitString(const std::string& str, const char delimiter)
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

    bool Utils::startsWith(const std::string& source, const std::string& str)
    {
        if(source.length() < str.length()) return false;

        for(int i = 0; i < str.length(); ++i)
        {
            if(source[i] != str[i]) return false;
        }
        return true;
    }

    std::string Utils::createKeyValuePair(std::string key, std::string value)
    {
        return std::string(std::move(key) + keyValueSeparator + std::move(value));
    }

    std::pair<std::string, std::string> Utils::parseKeyValuePair(const std::string& line)
    {
        size_t pos = line.find(keyValueSeparator);
        std::pair<std::string, std::string> result(line.substr(0, pos), line.substr(pos + 1));
        return std::pair(line.substr(0, pos), line.substr(pos + 1));
    }

    std::string Utils::serializeVec2(const glm::vec2& vec)
    {
        return std::to_string(vec.x) + vecSeparator + std::to_string(vec.y);
    }

    std::string Utils::serializeVec3(const glm::vec3& vec)
    {
        return std::to_string(vec.x) + vecSeparator + std::to_string(vec.y) + vecSeparator + std::to_string(vec.z);
    }

    std::string Utils::serializeVec4(const glm::vec4& vec)
    {
        return std::to_string(vec.x) + vecSeparator + std::to_string(vec.y) + vecSeparator + std::to_string(vec.z) + vecSeparator + std::to_string(vec.a);
    }

    glm::vec2 Utils::deserializeVec2(const std::string& line)
    {
        std::vector<std::string> values = Utils::splitString(line, vecSeparator);
        return glm::vec2(std::stof(values[0]), std::stof(values[1]));
    }

    glm::vec3 Utils::deserializeVec3(const std::string& line)
    {
        std::vector<std::string> values = Utils::splitString(line, vecSeparator);
        return glm::vec3(std::stof(values[0]), std::stof(values[1]), std::stof(values[2]));
    }

    glm::vec4 Utils::deserializeVec4(const std::string& line)
    {
        std::vector<std::string> values = Utils::splitString(line, vecSeparator);
        return glm::vec4(std::stof(values[0]), std::stof(values[1]), std::stof(values[2]), std::stof(values[3]));
    }

    std::string Utils::toLower(const std::string& str)
    {
        std::string lowerStr;
        lowerStr.reserve(str.size());
        for (const char& c : str) lowerStr.push_back(std::tolower(c));
        return lowerStr;
    }

    fs::path Utils::getAppDirPath()
    {
        static fs::path path;
        if(path.empty())
        {
            #ifdef _WIN32
                #ifdef UNICODE
                    WCHAR pathArr[400];
                    GetModuleFileName(NULL, pathArr, (sizeof(pathArr)));
                    path = fs::path(std::wstring(pathArr));
                #else
                    CHAR pathArr[400];
                    GetModuleFileName(NULL, pathArr, (sizeof(pathArr)));
                    path = fs::path(std::string(pathArr));
                #endif // ifdef UNICODE
            #else
                path = fs::canonical("/proc/self/exe");
            #endif

            path = path.parent_path();
        }
        return path;
    }

    std::string Utils::currentDateTimeString(const char* format)
    {
        std::time_t time = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
        std::string timeStr(30, '\0');
        std::strftime(&timeStr[0], timeStr.size(), format, std::localtime(&time));
        return timeStr;
    }
}