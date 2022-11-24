#include "resource_manager.hpp"
#include "typedefs.hpp"

namespace prim
{
    std::string ResourceManager::createResourcePath(std::string resPath) 
    {
        return (fs::path(resDirPath) / resPath).string();
    }

    std::string ResourceManager::getFontFilePath(std::string filename) 
    {
        return resDirPath + fontsDirPath + filename;
    }
    
    std::string ResourceManager::getTextureFilePath(std::string filename) 
    {
        return resDirPath + texturesDirPath + filename;
    }
    
    std::string ResourceManager::getScenePath(std::string filename) 
    {
        return resDirPath + scenesDirPath + filename;
    }
    
    std::string ResourceManager::getShaderFilePath(std::string filename) 
    {
        return resDirPath + shadersDirPath + filename;
    }
    
    std::string ResourceManager::getDefaultFontFilePath() 
    {
        return (getFontFilePath(defaultFontFilename));
    }
    
    std::string ResourceManager::getDefaultShaderFilePath() 
    {
        return (getShaderFilePath(defaultShaderFilename));
    }
    
    std::string ResourceManager::getDefaultTextShaderFilePath() 
    {
        return (getShaderFilePath(defaultTextShaderFilename));
    }

    
} // namespace prim
