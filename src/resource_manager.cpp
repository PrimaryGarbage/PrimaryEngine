#include "resource_manager.hpp"
#include "typedefs.hpp"
#include "prim_exception.hpp"
#include "utils.hpp"

namespace prim
{
    bool ResourceManager::validResourcePath(std::string path) 
    {
        return Utils::startsWith(path, resDirPath);
    }

    std::string ResourceManager::createResourcePath(std::string resPath, bool exists) 
    {
        if(validResourcePath(resPath)) return resPath;
        fs::path path = Utils::getAppDirPath() / resDirPath / resPath;
        path = path.make_preferred();
        if(!fs::exists(path) && exists)
            throw PRIM_EXCEPTION("Resource with the given path doesn't exists. Path: " + path.string());
        return path.string();
    }
    
    std::string ResourceManager::getResourceDirPathAbsolute() 
    {
        return (Utils::getAppDirPath() / resDirPath).string();
    }
    
    bool ResourceManager::resourceExists(std::string resPath) 
    {
        return fs::exists(fs::path(createResourcePath(resPath)));
    }
    
    char ResourceManager::separator() 
    {
        #ifdef _WIN32
            return '\\';
        #else
            return '/';
        #endif
    }

} // namespace prim
