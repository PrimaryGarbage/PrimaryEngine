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
#include <iostream>

namespace prim
{

	PrimaryApp::PrimaryApp() : deltaTime(0.0f), timeSinceStart(0.0f), sceneManager("./res/scenes/")
	{
	}

	PrimaryApp::~PrimaryApp()
	{
		Logger::terminate();
		if (currentScene) sceneManager.freeScene(currentScene);
	}

	void PrimaryApp::init()
	{
		Logger::init("./");
		renderer.init(windowWidth, windowHeight, windowName);
		Input::init(renderer.getWindow());
		initEditor();
		initGlobals();
	}

	int PrimaryApp::run()
	{
		try
		{
			init();

			timer.start();

			mainLoop();

			return 0;
		}
		catch (const prim::Exception& e)
		{
			std::cerr << e.what() << '\n';
			Logger::log(e.what(), true);
			return 1;
		}
		catch (const std::exception& e)
		{
			std::cerr << e.what() << '\n';
			Logger::log(e.what(), true);
			return 1;
		}
		catch (...)
		{
			std::cerr << "Unknown exception was caught!" << '\n';
			Logger::log("Unknown exception was caught!");
			return 1;
		}
	}

	void PrimaryApp::setCurrentScene(Node* scene)
	{
		if (currentScene) sceneManager.freeScene(currentScene);
		currentScene = scene;
		currentScene->start();
	}

	void PrimaryApp::loadCurrentScene(std::string name)
	{
		setCurrentScene(sceneManager.loadScene(name));
	}
	
	void PrimaryApp::saveCurrentScene(std::string name, bool overwrite)
	{
		if(!currentScene) return;
		sceneManager.saveScene(currentScene, name, overwrite);
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
		if (nodePath.front() == currentScene->getName())
			nodePath = nodePath.pop_front();
	start:
		while (!nodePath.empty())
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
		Node* scene1 = sceneManager.loadScene("TestScene1");
		//Node* scene1 = TestScenesCreator::createScene1();
		setCurrentScene(scene1);

		float speed = 10.0f;

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

			if (currentScene)
				currentScene->update(deltaTime);

			executeDeferredFunctions();
			/////////////////

			///// Draw /////

			currentScene->draw(renderer);

			drawEditor();

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

	void PrimaryApp::drawEditor()
	{
		if(!useEditor) return;
		sceneEditor.draw();
	}

	void PrimaryApp::initEditor()
	{
		if(!useEditor) return;
		sceneEditor.init(&renderer);
	}
	
	void PrimaryApp::initGlobals()
	{
		Globals::app = this;
		Globals::mainRenderer = &renderer;
		Globals::sceneEditor = &sceneEditor;
		Globals::sceneManager = &sceneManager;
	}

}