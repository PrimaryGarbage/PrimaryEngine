#ifndef __PRIMARY_APP_HPP__
#define __PRIMARY_APP_HPP__

#include "GL/glew.h"
#include "GLFW/glfw3.h"
#include <filesystem>

class PrimaryApp
{
private:
	const uint32_t windowWidth = 800;
	const uint32_t windowHeight = 800;
	const char* windowName = "PrimaryEngine";

	void mainLoop();
	static void framebuffer_size_callback(GLFWwindow* window, int width, int height);
	static void error_callback(int error, const char* description);

public:
	explicit PrimaryApp(const char* appPath);
	PrimaryApp(const PrimaryApp& other) = delete;
	~PrimaryApp();

	GLFWwindow* window = nullptr;
	const std::filesystem::path appPath;

	void init();
	void run();
};

#endif  // __PRIMARY_APP_HPP__
