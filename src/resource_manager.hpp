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

        static inline const char* defaultFontFilename = "Roboto-Regular.ttf";
        static inline const char* defaultShaderFilename = "default.shader";
        static inline const char* defaultTextShaderFilename = "text_default.shader";
    public:
        static std::string createResourcePath(std::string resPath);

        static std::string getFontFilePath(std::string filename);
        static std::string getTextureFilePath(std::string filename);
        static std::string getScenePath(std::string filename);
        static std::string getShaderFilePath(std::string filename);

        static std::string getDefaultFontFilePath();
        static std::string getDefaultShaderFilePath();
        static std::string getDefaultTextShaderFilePath();
    };
} // namespace prim


#endif // __RESOURCE_MANAGER_HPP__