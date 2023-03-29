#ifndef __SHADER_HPP__
#define __SHADER_HPP__

#include <string>
#include <unordered_map>
#include "GLM/glm.hpp"
#include "typedefs.hpp"


namespace prim
{
    struct ShaderProgramSource;

    enum class DefaultShader { simple, plainColor, frame, text, controlBackground };

    class Shader
    {
    private:
        inline static unsigned int currentBoundShader = 0u;
        inline static std::unordered_map<std::string, Unp<Shader>> shaderCache;
        inline static Unp<Shader> defaultShader;
        inline static Unp<Shader> defaultPlainColorShader;
        inline static Unp<Shader> frameShader;
        inline static Unp<Shader> defaultTextShader;
        inline static Unp<Shader> defaultControlBackgroundShader;

        unsigned int gl_id;
        mutable std::unordered_map<std::string, int> uniformLocationCache;

        int getUniformLocation(const std::string name) const;
        unsigned int compileShader(unsigned int type, const std::string source);
        ShaderProgramSource parseShader(std::string filePath);
        ShaderProgramSource parseShader(const char* fileText);
        unsigned int createShaderProgram(const std::string vertexShader, const std::string fragmentShader);

        void unload();

        Shader(const std::string& filePath);
        Shader(const char* fileText);
    public:
        ~Shader();

        static Shader* create(std::string resPath);
        static Shader* getDefaultShader(DefaultShader shaderType = DefaultShader::simple);
        static void terminate();

        void bind() const;
        void unbind() const;

        void setUniformMat4f(const std::string name, const glm::mat4 matrix) const;
        void setUniform4f(const std::string name, float v0, float v1, float v2, float v3) const;
        void setUniform4f(const std::string name, glm::vec4 vec) const;
        void setUniform1f(const std::string name, float value) const;
        void setUniform1i(const std::string name, int value) const;
        void setUniform2f(const std::string name, glm::vec2 vec) const;

        inline unsigned int getId() const { return gl_id; }
    };

}

#endif // __SHADER_HPP__