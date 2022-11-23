#include "shader_factory.hpp"
#include "resource_manager.hpp"

namespace prim
{
    ShaderFactory::~ShaderFactory() 
    {
        for(auto& pair : shaderCache)
        {
            delete pair.second;
        }
    }

    Shader* ShaderFactory::createShader(std::string filename) 
    {
        auto iter = shaderCache.find(filename);
        if(iter == shaderCache.end())
        {
            Shader* shader = new Shader(ResourceManager::getShaderFilePath(filename));
            shaderCache[filename] = shader;
            return shader;
        }
        else
        {
            return iter->second;
        }
    }

    
} // namespace prim
