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

PrimaryApp::PrimaryApp(const char* appPath) : appPath(appPath), deltaTime(0.0f), timeSinceStart(0.0f)
{
}

PrimaryApp::~PrimaryApp()
{
	Logger::terminate();
}

void PrimaryApp::init()
{
	Logger::init(appPath);
	renderer.init(windowWidth, windowHeight, windowName);
	Input::init(renderer.getWindow());

	Globals::app = this;
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
	Node scene("TestScene");

	Sprite sprite1("testSprite", "res/textures/TestTexture.png");
	Sprite sprite2("testSprite", "res/textures/TestTexture.png");
	Sprite background("background", "res/textures/abstract_stairs.png");

	ActorCamera2D camera("Player camera", &renderer, &sprite1);
	camera.move(0.0f, -100.0f);
	camera.setStiffness(0.01f);
	background.setSize(1920.0f, 1080.0f);
	background.setCenterPivot();
	sprite1.setCenterPivot();
	sprite2.setCenterPivot();
	sprite2.move(200.0f, 200.0f);
	sprite2.setZIndex(1.0f);
	float speed = 10.0f;

	scene.addChild(&background);
	scene.addChild(&sprite1);
	scene.addChild(&sprite2);
	scene.addChild(&camera);

	setCurrentScene(&scene);

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

		sprite2.lookAtSmooth(sprite1.getGlobalPosition(), 0.05f);

		sprite1.move(Input::getAxis("Vertical") * speed * sprite1.forward());
		sprite1.rotate(-Input::getAxis("Horizontal") * 0.03); 

		if(Input::isPressed(Key::comma)) background.scale(0.99f);
		if(Input::isPressed(Key::period)) background.scale(1.01f);
		
		executeDeferredFunctions();
		/////////////////

		///// Draw /////

		currentScene->draw(renderer);

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