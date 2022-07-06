#ifndef __SHADER_HPP__
#define __SHADER_HPP__

#include <string>
#include <unordered_map>

namespace prim
{
    struct ShaderProgramSource;

    class Shader
    {
    private:
        unsigned int gl_id;
        std::string filePath;
        std::unordered_map<std::string, int> uniformLocationCache;

        int getUniformLocation(const std::string name);
        unsigned int compileShader(unsigned int type, const std::string source);

    public:
        Shader(const std::string &filePath);
        Shader(const Shader& other) = delete;
        Shader(Shader&& other);
        Shader& operator=(Shader&& other);
        ~Shader();

        void bind() const;
        void unbind() const;
        void setUniform4f(const std::string name, float v0, float v1, float v2, float v3);
        void setUniform1f(const std::string name, float value);
        void setUniform1i(const std::string name, int value);
        ShaderProgramSource parseShader(std::string filePath);
        unsigned int createShaderProgram(const std::string vertexShader, const std::string fragmentShader);
    };

}

#endif // __SHADER_HPP__