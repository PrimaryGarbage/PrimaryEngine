#include "shader.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include "prim_exception.hpp"
#include "resource_manager.hpp"
#include "default_shader_data.hpp"
#include "glfw_extensions.hpp"
#include "globals.hpp"

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
            shaderCache[resPath] = Unp<Shader>(new Shader(ResourceManager::createResourcePath(resPath)));
            Logger::inst().logInfo("Shader loaded. Path: " + resPath);
            return shaderCache[resPath].get();
        }

        return it->second.get();
    }
    
    Shader* Shader::getDefaultShader(DefaultShader shaderType) 
    {
        switch(shaderType)
        {
            case DefaultShader::simple:
            {
                if(!defaultShader) 
                {
                    defaultShader = Unp<Shader>(new Shader(defaultShaderData));
                    Logger::inst().logInfo("Loaded default shader");
                }
                return defaultShader.get();
            }
            case DefaultShader::frame:
            {
                if(!frameShader) 
                {
                    frameShader = Unp<Shader>(new Shader(frameShaderData));
                    Logger::inst().logInfo("Loaded frame shader");
                }
                return frameShader.get();
            }
            case DefaultShader::text:
            {
                if(!defaultTextShader) 
                {
                    defaultTextShader = Unp<Shader>(new Shader(defaultTextShaderData));
                    Logger::inst().logInfo("Loaded text default shader");
                }
                return defaultTextShader.get();
            }
            case DefaultShader::plainColor:
            {
                if(!defaultPlainColorShader)
                {
                    defaultPlainColorShader = Unp<Shader>(new Shader(defaultPlainColorShaderData));
                    Logger::inst().logInfo("Loaded default plain color shader");
                }
                return defaultPlainColorShader.get();
            }
            case DefaultShader::controlBackground:
            {
                if(!defaultControlBackgroundShader)
                {
                    defaultControlBackgroundShader = Unp<Shader>(new Shader(defaultControlBackgroundShaderData));
                    Logger::inst().logInfo("Loaded default control background shader");
                }
                return defaultControlBackgroundShader.get();
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
            pair.second.reset();
            Logger::inst().logInfo("Shader terminated. Path: " + pair.first);
        }
        shaderCache.clear();

        if(defaultShader) defaultShader.reset();
        if(frameShader) frameShader.reset();
        if(defaultTextShader) defaultTextShader.reset();
        if(defaultPlainColorShader) defaultPlainColorShader.reset();
        if(defaultControlBackgroundShader) defaultControlBackgroundShader.reset();
        Logger::inst().logInfo("Default shaders terminated.");
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
            Logger::inst().logWarning("Warning: uniform '" + name + "' doesn't exist!", true);

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
            Logger::inst().logError("Failed to compile shader. Shader type: " + std::to_string(type), true);
            Logger::inst().logError(message, true);
            glDeleteShader(id);
            throw PRIM_EXCEPTION("Failed to compile shader Shader type: " + std::to_string(type) + std::string(std::move(message)));
        }

        return id;
    }

    void Shader::unload()
    {
        if (gl_id > 0)
        {
            unbind();
            GL_CALL(glDeleteProgram(gl_id));
            gl_id = 0;
        }
    }
}