#ifndef __RESOURCE_MANAGER_HPP__
#define __RESOURCE_MANAGER_HPP__

#include <string>

namespace prim
{
    class ResourceManager
    {
    private:
        static inline const std::string resDirPath = "./res/";
        static inline const char* fontsDirPath = "fonts/";
        static inline const char* scenesDirPath = "scenes/";
        static inline const char* shadersDirPath = "shaders/";
        static inline const char* texturesDirPath = "textures/";
    public:
        static std::string getFontFilePath(std::string filename);
        static std::string getTextureFilePath(std::string filename);
        static std::string getScenePath(std::string filename);
        static std::string getShaderFilePath(std::string filename);
    };
} // namespace prim


#endif // __RESOURCE_MANAGER_HPP__