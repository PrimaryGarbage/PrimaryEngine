#include "primary_app.hpp"
#include "logger.hpp"
#include "prim_exception.hpp"
#include "input.hpp"
#include <stdexcept>
#include <cassert>
#include <filesystem>
#include <algorithm>
#include "GLM/glm.hpp"
#include "globals.hpp"
#include "utils.hpp"
#include "nodes/node.hpp"
#include <iostream>
#include "macros.hpp"


namespace prim
{

	PrimaryApp::PrimaryApp() : deltaTime(0.0f), elapsedTime(0.0f)
	{
	}

	PrimaryApp::~PrimaryApp()
	{
		if (currentScene) sceneManager.freeScene(currentScene);
	}

	void PrimaryApp::init()
	{
		if(Globals::appStatus.appInitialized) return;

		initGlobals();
		renderer.init(windowWidth, windowHeight, windowName);
		Input::init(renderer.getWindow());
		initEditor();

		Globals::appStatus.appInitialized = true;
	}

	int PrimaryApp::run()
	{
		ASSERT_APP_INITIALIZED
		try
		{
			timer.start();

			mainLoop();

			return 0;
		}
		catch (const prim::Exception& e)
		{
			std::cerr << e.what() << '\n';
			logger.logError(e.what(), true);
			return 1;
		}
		catch (const std::exception& e)
		{
			std::cerr << e.what() << '\n';
			logger.logError(e.what(), true);
			return 1;
		}
		catch (...)
		{
			std::cerr << "Unknown exception was caught!" << '\n';
			logger.logError("Unknown exception was caught!");
			return 1;
		}
	}

	void PrimaryApp::setCurrentScene(Node* scene)
	{
		ASSERT_APP_INITIALIZED
		if (currentScene) sceneManager.freeScene(currentScene);
		currentScene = scene;
		currentScene->start();
	}

	void PrimaryApp::loadCurrentScene(std::string resPath)
	{
		ASSERT_APP_INITIALIZED
		setCurrentScene(sceneManager.loadScene(resPath));
	}
	
	void PrimaryApp::saveCurrentScene(std::string resPath, bool overwrite)
	{
		ASSERT_APP_INITIALIZED
		if(!currentScene) return;
		sceneManager.saveScene(currentScene, resPath, overwrite);
	}
	
	Node* PrimaryApp::loadScene(std::string resPath)
	{
		ASSERT_APP_INITIALIZED
		return sceneManager.loadScene(resPath);
	}

	Node* PrimaryApp::getCurrentScene() const
	{
		ASSERT_APP_INITIALIZED
		return currentScene;
	}

	Node* PrimaryApp::getNode(NodePath nodePath)
	{
		ASSERT_APP_INITIALIZED
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
	
	float PrimaryApp::getDeltaTime() const
	{
		ASSERT_APP_INITIALIZED
		return deltaTime;
	}
	
	float PrimaryApp::getElapsedTime() const
	{
		ASSERT_APP_INITIALIZED
		return elapsedTime;
	}

	void PrimaryApp::deferFunctionExecution(deferred_func_type function, short order)
	{
		ASSERT_APP_INITIALIZED
		deferredFunctions.push_back(std::pair<deferred_func_type, short>(function, order));
	}

	void PrimaryApp::mainLoop()
	{
		if(!currentScene) setCurrentScene(sceneManager.createEmptyScene());

		float speed = 10.0f;

		while (!renderer.windowShouldClose())
		{
			deltaTime = timer.peekSinceLastPeek() * 0.001f;
			elapsedTime = timer.peek() * 0.001f;

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
		Globals::logger = &logger;
	}
}