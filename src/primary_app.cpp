#include "primary_app.hpp"
#include "logger.hpp"
#include "prim_exception.hpp"
#include "texture.hpp"
#include <stdexcept>
#include <cassert>

namespace prim
{

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

	Shader shader("res/shaders/default.shader");
	shader.bind();
	shader.setUniform4f("u_color", 0.8f, 0.6f, 0.4f, 1.0f);

	Texture texture("res/textures/TestTexture.png");
	texture.bind();
	shader.setUniform1i("u_texture", 0);

	//////////////////

	mainLoop();
}

void PrimaryApp::mainLoop()
{
	while(!glfwWindowShouldClose(window))
	{
		renderer.clear();
		///// Draw /////

		
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

}

