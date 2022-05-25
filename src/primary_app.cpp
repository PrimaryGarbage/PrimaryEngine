#include "primary_app.hpp"
#include <stdexcept>
#include <iostream>

PrimaryApp::PrimaryApp()
{
	
}

PrimaryApp::~PrimaryApp()
{
	glfwTerminate();
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

	glfwSetErrorCallback(error_callback);
}

void PrimaryApp::run()
{
	mainLoop();
}

void PrimaryApp::mainLoop()
{
	while(!glfwWindowShouldClose(window))
	{
		glClear(GL_COLOR_BUFFER_BIT);

		// create VAO and set it as the current one
		GLuint VertexArrayID;
		glGenVertexArrays(1, &VertexArrayID);
		glBindVertexArray(VertexArrayID);


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
	std::cerr << "GLFW error: " << error << std::endl;
	std::cerr << description;
}