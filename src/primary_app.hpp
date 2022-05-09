#include <GL/glew.h>
#include <GLFW/glfw3.h>

class PrimaryApp
{
public:
	explicit PrimaryApp();
	PrimaryApp(const PrimaryApp& other) = delete;
	~PrimaryApp();

	GLFWwindow* window = nullptr;

	void run();

private:
	const uint32_t WINDOW_WIDTH = 800;
	const uint32_t WINDOW_HEIGHT = 800;
	const char* WINDOW_NAME = "PrimaryEngine";

	void mainLoop();
};