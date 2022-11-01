#ifndef __PRIMARY_APP_HPP__
#define __PRIMARY_APP_HPP__

#include <filesystem>
#include <functional>
#include "renderer.hpp"
#include "timer.hpp"
#include "scene_editor.hpp"
#include "scene_manager.hpp"

namespace prim
{

typedef std::function<void()> deferred_func_type;

class PrimaryApp
{
private:
	const uint32_t windowWidth = 800;
	const uint32_t windowHeight = 800;
	const char* windowName = "PrimaryEngine";
	float deltaTime;
	float timeSinceStart;
	Logger logger;
	Timer timer;
	Renderer renderer;
	SceneEditor sceneEditor;
	Node* currentScene = nullptr;
	SceneManager sceneManager;
	std::vector<std::pair<deferred_func_type, short>> deferredFunctions;

	void mainLoop();
	void executeDeferredFunctions();
	void drawEditor();
	void initEditor();
	void initGlobals();
public:
	bool useEditor = false;

	PrimaryApp();
	PrimaryApp(const PrimaryApp& other) = delete;
	~PrimaryApp();

	void init();
	int run();

	void setCurrentScene(Node* scene);
	void loadCurrentScene(std::string name);
	void saveCurrentScene(std::string name, bool overwrite = false);
	Node* loadScene(std::string name);
	Node* getCurrentScene() const;
	Node* getNode(NodePath nodePath);

	void deferFunctionExecution(deferred_func_type function, short order = 1);
};

}


#endif // __PRIMARY_APP_HPP__