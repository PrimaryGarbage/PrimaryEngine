#include <filesystem>
#include "renderer.hpp"
#include "timer.hpp"

namespace prim
{

class PrimaryApp
{
private:
	const uint32_t windowWidth = 800;
	const uint32_t windowHeight = 800;
	const char* windowName = "PrimaryEngine";
	float deltaTime;
	float timeSinceStart;
	Timer timer;
	Renderer renderer;

	void mainLoop();
public:
	explicit PrimaryApp(const char* appPath);
	PrimaryApp(const PrimaryApp& other) = delete;
	~PrimaryApp();

	const std::filesystem::path appPath;

	void init();
	void run();
};

}

