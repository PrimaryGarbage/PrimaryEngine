#include "GL/glew.h"
#include "GLFW/glfw3.h"
#include <filesystem>
#include "renderer.hpp"

namespace prim
{

class PrimaryApp
{
private:
	const uint32_t windowWidth = 800;
	const uint32_t windowHeight = 800;
	const char* windowName = "PrimaryEngine";
	Renderer renderer;

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

}

