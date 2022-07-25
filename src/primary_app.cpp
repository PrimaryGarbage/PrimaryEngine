#include "primary_app.hpp"
#include "logger.hpp"
#include "prim_exception.hpp"
#include "input.hpp"
#include <stdexcept>
#include <cassert>
#include <filesystem>
#include <algorithm>
#include "glm.hpp"
#include "globals.hpp"
#include "test_scenes_creator.hpp"
#include "utils.hpp"

namespace prim
{

	PrimaryApp::PrimaryApp(const char* appPath) : appPath(appPath), deltaTime(0.0f), timeSinceStart(0.0f), sceneManager("./res/scenes/")
	{
	}

	PrimaryApp::~PrimaryApp()
	{
		Logger::terminate();
		if (currentScene) sceneManager.freeScene(currentScene);
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
		if(currentScene) sceneManager.freeScene(currentScene);
		currentScene = scene;
		currentScene->start();
	}
	
	void PrimaryApp::loadCurrentScene(std::string name) 
	{
		setCurrentScene(sceneManager.loadScene(name));
	}
	
	Node* PrimaryApp::loadScene(std::string name) 
	{
		return sceneManager.loadScene(name);
	}

	Node* PrimaryApp::getCurrentScene() const
	{
		return currentScene;
	}

	Node* PrimaryApp::getNode(NodePath nodePath)
	{
		if (!currentScene) return nullptr;
		Node* currentNode = currentScene;
		if(nodePath.front() == currentScene->getName())
			nodePath = nodePath.pop_front();
		start:
		while(!nodePath.empty())
		{
			std::string name = nodePath.front();
			for (Node* child : currentNode->getChildren())
			{
				if (name == child->getName())
				{
					currentNode = child;
					nodePath = nodePath.pop_front();
					goto start;
					break;
				}
			}
			return nullptr;
		}
		return currentNode;
	}

	void PrimaryApp::deferFunctionExecution(deferred_func_type function, short order)
	{
		deferredFunctions.push_back(std::pair<deferred_func_type, short>(function, order));
	}

	void PrimaryApp::mainLoop()
	{
		// Node* scene1 = TestScenesCreator::createScene1();
		// sceneManager.saveScene(scene1, "TestScene1", true);
		// setCurrentScene(scene1);

		Node* scene1 = sceneManager.loadScene("TestScene1");
		setCurrentScene(scene1);

		float speed = 10.0f;

		Sprite* sprite = currentScene->findChild<Sprite>("TestSprite1");
		ActorCamera2D* camera = currentScene->findChild<ActorCamera2D>();



		while (!renderer.windowShouldClose())
		{
			deltaTime = timer.peekSinceLastPeek() * 0.001f;
			timeSinceStart = timer.peek() * 0.001f;

			renderer.clear();

			///// Update /////
			deferredFunctions.clear();

			Input::clear();
			renderer.pollEvents();
			Input::update();

			sprite->move(sprite->forward() * Input::getAxis("Vertical") * speed);
			sprite->rotate(-Input::getAxis("Horizontal") * 0.05f);

			if (currentScene)
				currentScene->update(deltaTime);

			mainUI.print(Utils::serializeVec2(camera->getGlobalPosition()));

			executeDeferredFunctions();
			/////////////////

			///// Draw /////

			currentScene->draw(renderer);

			mainUI.draw();

			////////////////

			renderer.swapBuffers();
		}
	}

	void PrimaryApp::executeDeferredFunctions()
	{
		std::sort(deferredFunctions.begin(), deferredFunctions.end(), [](const auto& pair1, const auto& pair2) { return pair1.second < pair2.second; });
		for (int i = 0; i < deferredFunctions.size(); ++i)
			deferredFunctions[i].first();
	}

}