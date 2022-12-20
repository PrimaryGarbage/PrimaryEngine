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
        inline static unsigned int currentBoundShader = 0u;
        inline static std::unordered_map<std::string, Shader*> shaderCache;
        inline static Shader* defaultShader = nullptr;
        inline static Shader* selectShader = nullptr;
        inline static Shader* textDefaultShader = nullptr;

        unsigned int gl_id;
        mutable std::unordered_map<std::string, int> uniformLocationCache;

        int getUniformLocation(const std::string name) const;
        unsigned int compileShader(unsigned int type, const std::string source);
        ShaderProgramSource parseShader(std::string filePath);
        ShaderProgramSource parseShader(const char* fileText);
        unsigned int createShaderProgram(const std::string vertexShader, const std::string fragmentShader);

        void unload();

        Shader(const std::string &filePath);
        Shader(const char* fileText);
    public:
        ~Shader();

        static Shader* create(std::string resPath);
        static Shader* getDefaultShader();
        static Shader* getSelectShader();
        static Shader* getTextDefaultShader();
        static void terminate();

        void bind() const;
        void unbind() const;

        void setUniformMat4f(const std::string name, const glm::mat4 matrix) const;
        void setUniform4f(const std::string name, float v0, float v1, float v2, float v3) const;
        void setUniform1f(const std::string name, float value) const;
        void setUniform1i(const std::string name, int value) const;

        inline unsigned int getId() const { return gl_id; }
    };

}

#endif // __SHADER_HPP__