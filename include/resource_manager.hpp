#ifndef __RESOURCE_MANAGER_HPP__
#define __RESOURCE_MANAGER_HPP__

#include <string>

namespace prim
{
    class ResourceManager
    {
    private:
        static bool validResourcePath(std::string path);
    public:
        static inline const std::string resDirName = "resources";
        static inline const std::string resDirPath = "./" + resDirName + "/";

        static std::string createResourcePath(std::string resPath, bool exists = false);
        static std::string getResourceDirPathAbsolute();
        static bool resourceExists(std::string resPath);
        static char separator();
    };
} // namespace prim


#endif // __RESOURCE_MANAGER_HPP__