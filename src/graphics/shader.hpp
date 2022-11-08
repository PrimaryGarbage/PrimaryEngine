#ifndef __SHADER_HPP__
#define __SHADER_HPP__

#include <string>
#include <unordered_map>
#include "glm.hpp"



namespace prim
{
    struct ShaderProgramSource;

    class Shader
    {
    private:
        inline static unsigned int currentShader = 0u;

        unsigned int gl_id;
        std::string filePath;
        mutable std::unordered_map<std::string, int> uniformLocationCache;

        int getUniformLocation(const std::string name) const;
        unsigned int compileShader(unsigned int type, const std::string source);

        void unload();
    public:
        Shader(const std::string &filePath);
        Shader(const Shader& other) = delete;
        Shader(Shader&& other);
        Shader& operator=(Shader&& other);
        ~Shader();

        void bind() const;
        void unbind() const;
        void setUniformMat4f(const std::string name, const glm::mat4 matrix) const;
        void setUniform4f(const std::string name, float v0, float v1, float v2, float v3) const;
        void setUniform1f(const std::string name, float value) const;
        void setUniform1i(const std::string name, int value) const;
        ShaderProgramSource parseShader(std::string filePath);
        unsigned int createShaderProgram(const std::string vertexShader, const std::string fragmentShader);
        inline unsigned int getId() const { return gl_id; }
    };

}

#endif // __SHADER_HPP__