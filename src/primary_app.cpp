#include "primary_app.hpp"
#include "logger.hpp"
#include "prim_exception.hpp"
#include "input.hpp"
#include <stdexcept>
#include <cassert>
#include <filesystem>
#include <algorithm>
#include "glm.hpp"
#include "sprite.hpp"
#include "actor_camera2d.hpp"
#include "globals.hpp"

namespace prim
{

PrimaryApp::PrimaryApp(const char* appPath) : appPath(appPath), deltaTime(0.0f), timeSinceStart(0.0f), sceneManager("./res/scenes/")
{
}

PrimaryApp::~PrimaryApp()
{
	Logger::terminate();
	if(currentScene) sceneManager.freeScene(currentScene);
}

void PrimaryApp::init()
{
	Logger::init(appPath);
	renderer.init(windowWidth, windowHeight, windowName);
	Input::init(renderer.getWindow());
	mainUI.init(&renderer);

	Globals::app = this;
	Globals::mainRenderer = &renderer;
}

void PrimaryApp::run()
{
	timer.start();

	mainLoop();
}

void PrimaryApp::setCurrentScene(Node* scene)
{
	currentScene = scene;
	currentScene->start();
}

Node* PrimaryApp::getCurrentScene() const
{
	return currentScene;
}

void PrimaryApp::deferFunctionExecution(deferred_func_type function, short order)
{
	deferredFunctions.push_back(std::pair<deferred_func_type, short>(function, order));
}

void PrimaryApp::mainLoop()
{
	Node* scene = new Node("TestScene");
	setCurrentScene(scene);

	sceneManager.loadScene("testSceneSprite1", scene);

	while(!renderer.windowShouldClose())
	{
		deltaTime = timer.peekSinceLastPeek() * 0.001f;
		timeSinceStart = timer.peek() * 0.001f;

		renderer.clear();

		///// Update /////
		deferredFunctions.clear();

		Input::update();

		if(currentScene)
			currentScene->update(deltaTime);

		executeDeferredFunctions();
		/////////////////

		///// Draw /////

		currentScene->draw(renderer);

		mainUI.draw();

		////////////////

		renderer.swapBuffers();
		renderer.pollEvents();
	}
}

void PrimaryApp::executeDeferredFunctions()
{
	std::sort(deferredFunctions.begin(), deferredFunctions.end(), [](const auto& pair1, const auto& pair2) { return pair1.second < pair2.second; });
	for(int i = 0; i < deferredFunctions.size(); ++i)
		deferredFunctions[i].first();
}

}