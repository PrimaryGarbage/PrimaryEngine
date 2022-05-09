#include "primary_app.hpp"
#include <stdexcept>

PrimaryApp::PrimaryApp()
{
	if(!glfwInit()) { throw std::runtime_error("Failed to initialize glfw!"); }

	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_NAME, nullptr, nullptr);
	if(window == nullptr) { glfwTerminate(); throw std::runtime_error("Failed to create glfw window!"); }

	glfwMakeContextCurrent(window);
	glewExperimental = true;
	if(glewInit() != GLEW_OK)
	{
		glfwTerminate();
		throw std::runtime_error("Failed to initialize glew!");
	}

	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
}

PrimaryApp::~PrimaryApp()
{
	glfwTerminate();
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


		glfwSwapBuffers(window);
		glfwPollEvents();
	}
}