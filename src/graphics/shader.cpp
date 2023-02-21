#include "shader.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include "prim_exception.hpp"
#include "resource_manager.hpp"
#include "default_shader_data.hpp"
#include "macros.hpp"
#include "glfw_extensions.hpp"

namespace prim
{

    struct ShaderProgramSource
    {
        std::string vertexSource;
        std::string fragmentSource;
    };

    Shader::Shader(const std::string& filePath) : gl_id(0)
    {
        ShaderProgramSource source = parseShader(filePath);
        gl_id = createShaderProgram(source.vertexSource, source.fragmentSource);
    }
    
    Shader::Shader(const char* fileText) 
    {
        ShaderProgramSource source = parseShader(fileText);
        gl_id = createShaderProgram(source.vertexSource, source.fragmentSource);
    }

    Shader::~Shader()
    {
        unload();
    }
    
    Shader* Shader::create(std::string resPath) 
    {
        auto it = shaderCache.find(resPath);
        if(it == shaderCache.end())
        {
            Shader* shader = new Shader(ResourceManager::createResourcePath(resPath));
            shaderCache[resPath] = shader;
            Globals::logger->logInfo("Shader loaded. Path: " + resPath);
            return shader;
        }

        return it->second;
    }
    
    Shader* Shader::getDefaultShader(DefaultShader shaderType) 
    {
        switch(shaderType)
        {
            case DefaultShader::simple:
            {
                if(!defaultShader) 
                {
                    defaultShader = new Shader(defaultShaderData);
                    Globals::logger->logInfo("Loaded default shader");
                }
                return defaultShader;
            }
            case DefaultShader::frame:
            {
                if(!frameShader) 
                {
                    frameShader = new Shader(frameShaderData);
                    Globals::logger->logInfo("Loaded frame shader");
                }
                return frameShader;
            }
            case DefaultShader::text:
            {
                if(!defaultTextShader) 
                {
                    defaultTextShader = new Shader(defaultTextShaderData);
                    Globals::logger->logInfo("Loaded text default shader");
                }
                return defaultTextShader;
            }
            case DefaultShader::plainColor:
            {
                if(!defaultPlainColorShader)
                {
                    defaultPlainColorShader = new Shader(defaultPlainColorShaderData);
                    Globals::logger->logInfo("Loaded default plain color shader");
                }
                return defaultPlainColorShader;
            }
            case DefaultShader::controlBackground:
            {
                if(!defaultControlBackgroundShader)
                {
                    defaultControlBackgroundShader = new Shader(defaultControlBackgroundShaderData);
                    Globals::logger->logInfo("Loaded default control background shader");
                }
                return defaultControlBackgroundShader;
            }
            default:
            {
                throw PRIM_EXCEPTION("Trying to get unknown type of default shader");
            }
        }
    }
    
    void Shader::terminate() 
    {
        for(auto& pair : shaderCache)
        {
            delete pair.second;
            Globals::logger->logInfo("Shader terminated. Path: " + pair.first);
        }
        shaderCache.clear();

        if(defaultShader) delete defaultShader;
        if(frameShader) delete frameShader;
        if(defaultTextShader) delete defaultTextShader;
        if(defaultPlainColorShader) delete defaultPlainColorShader;
        if(defaultControlBackgroundShader) delete defaultControlBackgroundShader;
        Globals::logger->logInfo("Default shaders terminated.");
    }

    void Shader::bind() const
    {
        if(currentBoundShader != gl_id)
        {
            GL_CALL(glUseProgram(gl_id));
            currentBoundShader = gl_id;
        }
    }

    void Shader::unbind() const
    {
        GL_CALL(glUseProgram(0u));
        currentBoundShader = 0u;
    }

    int Shader::getUniformLocation(const std::string name) const
    {
        auto it = uniformLocationCache.find(name);
        if (it != uniformLocationCache.end())
            return it->second;

        GL_CALL(int location = glGetUniformLocation(gl_id, name.c_str()));
        if (location == -1)
            Globals::logger->logWarning("Warning: uniform '" + name + "' doesn't exist!", true);

        uniformLocationCache[name] = location;

        return location;
    }

    void Shader::setUniformMat4f(const std::string name, const glm::mat4 matrix) const
    {
        bind();
        GL_CALL(glUniformMatrix4fv(getUniformLocation(name), 1, GL_FALSE, &matrix[0][0]));
    }

    void Shader::setUniform4f(const std::string name, float v0, float v1, float v2, float v3) const
    {
        bind();
        GL_CALL(glUniform4f(getUniformLocation(name), v0, v1, v2, v3));
    }
    
    void Shader::setUniform4f(const std::string name, glm::vec4 vec) const
    {
        setUniform4f(name, vec.x, vec.y, vec.z, vec.a);
    }

    void Shader::setUniform1f(const std::string name, float value) const
    {
        bind();
        GL_CALL(glUniform1f(getUniformLocation(name), value));
    }

    void Shader::setUniform1i(const std::string name, int value) const
    {
        bind();
        GL_CALL(glUniform1i(getUniformLocation(name), value));
    }
    
    void Shader::setUniform2f(const std::string name, glm::vec2 vec) const
    {
        bind();
        GL_CALL(glUniform2f(getUniformLocation(name), vec.x, vec.y));
    }

    unsigned int Shader::createShaderProgram(const std::string vertexShader, const std::string fragmentShader)
    {
        unsigned int program = glCreateProgram();
        unsigned int vs = compileShader(GL_VERTEX_SHADER, std::move(vertexShader));
        unsigned int fs = compileShader(GL_FRAGMENT_SHADER, std::move(fragmentShader));

        glAttachShader(program, vs);
        glAttachShader(program, fs);
        glLinkProgram(program);
        glValidateProgram(program);

        glDeleteShader(vs);
        glDeleteShader(fs);

        return program;
    }

    ShaderProgramSource Shader::parseShader(const std::string filePath)
    {
        std::ifstream stream(filePath);

        enum class ShaderType
        {
            none = -1,
            vertex = 0,
            fragment = 1
        };

        std::string line;
        std::stringstream ss[2];
        ShaderType type = ShaderType::none;
        while (std::getline(stream, line))
        {
            if (line.find("#shader") != std::string::npos)
            {
                if (line.find("vertex") != std::string::npos)
                    type = ShaderType::vertex;
                else if (line.find("fragment") != std::string::npos)
                    type = ShaderType::fragment;
            }
            else if (type != ShaderType::none)
            {
                ss[(int)type] << line << '\n';
            }
        }

        return { ss[0].str(), ss[1].str() };
    }
    
    ShaderProgramSource Shader::parseShader(const char* fileText) 
    {
        std::stringstream stream(fileText);

        enum class ShaderType
        {
            none = -1,
            vertex = 0,
            fragment = 1
        };

        std::string line;
        std::stringstream ss[2];
        ShaderType type = ShaderType::none;
        while (std::getline(stream, line))
        {
            if (line.find("#shader") != std::string::npos)
            {
                if (line.find("vertex") != std::string::npos)
                    type = ShaderType::vertex;
                else if (line.find("fragment") != std::string::npos)
                    type = ShaderType::fragment;
            }
            else if (type != ShaderType::none)
            {
                ss[(int)type] << line << '\n';
            }
        }

        return { ss[0].str(), ss[1].str() };
    }

    unsigned int Shader::compileShader(unsigned int type, const std::string source)
    {
        unsigned int id = glCreateShader(type);
        const char* src = source.c_str();
        glShaderSource(id, 1, &src, nullptr);
        glCompileShader(id);

        int result;
        glGetShaderiv(id, GL_COMPILE_STATUS, &result); // compilation info
        if (result == GL_FALSE)
        {
            int length;
            glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
            char* message = (char*)alloca(length * sizeof(char));
            glGetShaderInfoLog(id, length, &length, message);
            Globals::logger->logError("Failed to compile shader. Shader type: " + std::to_string(type), true);
            Globals::logger->logError(message, true);
            glDeleteShader(id);
            throw PRIM_EXCEPTION("Failed to compile shader Shader type: " + std::to_string(type) + std::string(std::move(message)));
        }

        return id;
    }

    void Shader::unload()
    {
        ASSERT_GLFW_NOT_TERMINATED
        if (gl_id > 0)
        {
            unbind();
            GL_CALL(glDeleteProgram(gl_id));
            gl_id = 0;
        }
    }
}