#include "resource_manager.hpp"

namespace prim
{
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

    
} // namespace prim
