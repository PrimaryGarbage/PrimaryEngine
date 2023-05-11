#ifndef __PRIMARY_APP_HPP__
#define __PRIMARY_APP_HPP__

#include <filesystem>
#include <functional>
#include "graphics/renderer.hpp"
#include "timer.hpp"
#include "scene_editor.hpp"
#include "scene_manager.hpp"
#include "logger.hpp"
#include "nodes/node_path.hpp"
#include "primary_app_options.hpp"
#include "input.hpp"

namespace prim
{

// fordec
class Node;
//

typedef std::function<void()> deferred_func_type;

class PrimaryApp
{
private:
	float deltaTime;
	float elapsedTime;
	bool useEditor;
	Timer timer;
	Renderer renderer;
	Input input;
	SceneEditor sceneEditor;
	Node* currentScene = nullptr;
	SceneManager sceneManager;
	std::vector<std::pair<deferred_func_type, short>> deferredFunctions;

	void mainLoop();
	void executeDeferredFunctions();
	void drawEditor();
public:

	PrimaryApp(const PrimaryAppOptions& options);
	PrimaryApp(const PrimaryApp& other) = delete;
	~PrimaryApp();

	int run();
	void setCurrentScene(Node* scene);
	void loadCurrentScene(std::string resPath);
	void saveCurrentScene(std::string resPath, bool overwrite = false);
	Node* loadScene(std::string resPath);
	Node* getCurrentScene() const;
	Node* getNode(NodePath nodePath);
	float getDeltaTime() const;
	float getElapsedTime() const;
	void deferFunctionExecution(deferred_func_type function, short order = 1);
};

}


#endif // __PRIMARY_APP_HPP__