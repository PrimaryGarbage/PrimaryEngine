#include "primary_app.hpp"
#include <stdexcept>
#include <iostream>
#include <fstream>
#include <sstream>
#include "logger.hpp"
#include "prim_exception.hpp"
#include <cassert>
#include "renderer.hpp"
#include "vertex_array.hpp"


PrimaryApp::PrimaryApp(const char* appPath) : appPath(appPath)
{
	
}

PrimaryApp::~PrimaryApp()
{
	glfwTerminate();
	Logger::log("GLFW successfully terminated");
	Logger::terminate();
}

void PrimaryApp::init()
{
	if(!glfwInit()) { throw std::runtime_error("Failed to initialize glfw!"); }

	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	window = glfwCreateWindow(windowWidth, windowHeight, windowName, nullptr, nullptr);
	if(window == nullptr) { throw std::runtime_error("Failed to create glfw window!"); }

	glfwMakeContextCurrent(window);
	glewExperimental = true;
	glViewport(0, 0, windowWidth, windowHeight);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	if(glewInit() != GLEW_OK) { throw std::runtime_error("Failed to initialize glew!"); }

	Logger::init(appPath);
	glfwSetErrorCallback(error_callback);

	glfwSwapInterval(1);

	Logger::log("GLFW and GLEW initialized successfully", true);
	Logger::log("OpenGL version: " + std::string(reinterpret_cast<const char*>(glGetString(GL_VERSION))), true);
	Logger::log("GPU: " + std::string(reinterpret_cast<const char*>(glGetString(GL_RENDERER))), true);
}

//// TEMP ////
struct ShaderProgramSource
{
	std::string vertexSource;
	std::string fragmentSource;
};

//// TEMP ////
static ShaderProgramSource parseShader(const std::string filePath)
{
	std::ifstream stream(filePath);

	enum class ShaderType
	{
		none = -1, vertex = 0, fragment = 1
	};

	std::string line;
	std::stringstream ss[2];
	ShaderType type = ShaderType::none;
	while(std::getline(stream, line))
	{
		if(line.find("#shader") != std::string::npos)
		{
			if(line.find("vertex") != std::string::npos) type = ShaderType::vertex;
			else if(line.find("fragment") != std::string::npos) type = ShaderType::fragment;
		}
		else if(type != ShaderType::none)
		{
			ss[(int)type] << line << '\n';
		}
	}

	return { ss[0].str(), ss[1].str() };
}

//// TEMP ////
static unsigned int compileShader(unsigned int type, const std::string source)
{
	unsigned int id = glCreateShader(type);
	const char* src = source.c_str();
	glShaderSource(id, 1, &src, nullptr);
	glCompileShader(id);

	int result;
	glGetShaderiv(id, GL_COMPILE_STATUS, &result);
	if(result == GL_FALSE)
	{
		int length;
		glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
		char* message = (char*)alloca(length * sizeof(char));
		glGetShaderInfoLog(id, length, &length, message);
		Logger::log("Failed to compile shader. Shader type: " + std::to_string(type), true);
		Logger::log(message, true);
		glDeleteShader(id);
		throw PRIM_EXCEPTION("Failed to compile shader Shader type: " + std::to_string(type) + std::string(std::move(message)));
	}

	return id;
}

//// TEMP ////
static unsigned int createShader(const std::string vertexShader, const std::string fragmentShader)
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

void PrimaryApp::run()
{
	//// TEMP ////

	float positions[] = {
		-0.5f, -0.5f,
		0.5f, -0.5f,
		0.5f, 0.5f,
		-0.5f, 0.5f
	};

	unsigned int indices[] = {
		0, 1, 2,
		0, 2, 3
	};

	VertexArray va;
	VertexBuffer vb(positions, 8 * sizeof(float));
	VertexBufferLayout layout;
	layout.push<float>(2);
	va.addBuffer(vb, layout);

	IndexBuffer ib(indices, 6);

	ShaderProgramSource source = parseShader("res/shaders/default.shader");

	unsigned int shader = createShader(source.vertexSource, source.fragmentSource);
	glUseProgram(shader);

	int location = glGetUniformLocation(shader, "u_color");
	assert(location != -1);
	glUniform4f(location, 0.2f, 0.6f, 0.4f, 1.0f);

	////////////////

	mainLoop();

	//// TEMP ////
	glDeleteProgram(shader);
	////////////// 
}

void PrimaryApp::mainLoop()
{
	while(!glfwWindowShouldClose(window))
	{
		glClear(GL_COLOR_BUFFER_BIT);

		///// Draw /////

		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
		
		////////////////


		glfwSwapBuffers(window);
		glfwPollEvents();
	}
}

void PrimaryApp::framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void PrimaryApp::error_callback(int error, const char* description)
{
	Logger::log("GLFW error: " + std::to_string(error), true);
	Logger::log(description);
	// std::cerr << "GLFW error: " << error << std::endl;
	// std::cerr << description;
}