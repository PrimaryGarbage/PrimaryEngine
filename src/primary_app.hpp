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
public:
	explicit PrimaryApp(const char* appPath);
	PrimaryApp(const PrimaryApp& other) = delete;
	~PrimaryApp();

	const std::filesystem::path appPath;

	void init();
	void run();
};

}

