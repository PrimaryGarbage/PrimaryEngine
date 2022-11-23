#ifndef __SHADER_FACTORY_HPP__
#define __SHADER_FACTORY_HPP__

#include <unordered_map>
#include <string>
#include "shader.hpp"

namespace prim
{
    class ShaderFactory
    {
    private:
        std::unordered_map<std::string, Shader*> shaderCache;
    public:
        ~ShaderFactory();
        Shader* createShader(std::string filename);
    };
} // namespace prim


#endif // __SHADER_FACTORY_HPP__