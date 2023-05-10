#include "primary_app.hpp"
#include "logger.hpp"
#include "prim_exception.hpp"
#include <stdexcept>
#include <cassert>
#include <algorithm>
#include "GLM/glm.hpp"
#include "globals.hpp"
#include "utils.hpp"
#include "nodes/node.hpp"
#include <iostream>


namespace prim
{

	PrimaryApp::PrimaryApp(const PrimaryAppOptions& options) : 
		deltaTime(0.0f), elapsedTime(0.0f), useEditor(options.useEditor),
		renderer(options.windowWidth, options.windowHeight, options.windowName),
		input(renderer.getWindow()), sceneEditor(&renderer)
	{
		Globals::app = this;
		Globals::mainRenderer = &renderer;
		Globals::sceneEditor = &sceneEditor;
		Globals::sceneManager = &sceneManager;
	}

	PrimaryApp::~PrimaryApp()
	{
		if (currentScene) sceneManager.freeScene(currentScene);
	}

	int PrimaryApp::run()
	{
		try
		{
			timer.start();

			mainLoop();

			return 0;
		}
		catch (const prim::Exception& e)
		{
			std::cerr << e.what() << '\n';
			Logger::inst().logError(e.what(), true);
			return 1;
		}
		catch (const std::exception& e)
		{
			std::cerr << e.what() << '\n';
			Logger::inst().logError(e.what(), true);
			return 1;
		}
		catch (...)
		{
			std::cerr << "Unknown exception was caught!" << '\n';
			Logger::inst().logError("Unknown exception was caught!");
			return 1;
		}
	}

	void PrimaryApp::setCurrentScene(Node* scene)
	{
		if (currentScene) sceneManager.freeScene(currentScene);
		currentScene = scene;
		currentScene->start();
	}

	void PrimaryApp::loadCurrentScene(std::string resPath)
	{
		setCurrentScene(sceneManager.loadScene(resPath));
	}
	
	void PrimaryApp::saveCurrentScene(std::string resPath, bool overwrite)
	{
		if(!currentScene) return;
		sceneManager.saveScene(currentScene, resPath, overwrite);
	}
	
	Node* PrimaryApp::loadScene(std::string resPath)
	{
		return sceneManager.loadScene(resPath);
	}

	Node* PrimaryApp::getCurrentScene() const
	{
		return currentScene;
	}

	Node* PrimaryApp::getNode(NodePath nodePath)
	{
		if (!currentScene) return nullptr;

		Node* currentNode = nullptr;
		const std::vector<Node*>* currentLevel = &(currentScene->children);

	start:
		while(!nodePath.empty())
		{
			std::string currentName = nodePath.front();
			for(Node* node : *currentLevel)
			{
				if(node->getName() == currentName)
				{
					currentLevel = &node->children;
					currentNode = node;
					nodePath = nodePath.pop_front();
					goto start;
				}
			}

			return nullptr;
		}

		return currentNode;
	}
	
	float PrimaryApp::getDeltaTime() const
	{
		return deltaTime;
	}
	
	float PrimaryApp::getElapsedTime() const
	{
		return elapsedTime;
	}

	void PrimaryApp::deferFunctionExecution(deferred_func_type function, short order)
	{
		deferredFunctions.push_back(std::pair<deferred_func_type, short>(function, order));
	}

	void PrimaryApp::mainLoop()
	{
		float speed = 10.0f;

		while (!renderer.windowShouldClose())
		{
			deltaTime = timer.peekSinceLastPeek() * 0.001f;
			elapsedTime = timer.peek() * 0.001f;

			renderer.clear();

			///// Update /////

			Input::clear();
			renderer.pollEvents();
			Input::update();

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
		deferredFunctions.clear();
	}

	void PrimaryApp::drawEditor()
	{
		if(!useEditor) return;
		sceneEditor.draw();
	}
}