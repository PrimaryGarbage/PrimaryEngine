#include "primary_app.hpp"
#include "logger.hpp"
#include "prim_exception.hpp"
#include "input.hpp"
#include <stdexcept>
#include <cassert>
#include <filesystem>
#include "glm.hpp"
#include "gtc/matrix_transform.hpp"
#include "sprite.hpp"
#include "camera2d.hpp"
#include "primitives.hpp"

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
}

void PrimaryApp::run()
{
	//// TEMP ////

	glm::mat4 proj = glm::ortho(0.0f, static_cast<float>(windowWidth), 0.0f, static_cast<float>(windowHeight), -1.0f, 1.0f);

	renderer.setProjectMat(std::move(proj));

	//////////////////

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

void PrimaryApp::mainLoop()
{
	Node scene("TestScene");

	Camera2D camera("Player camera", &renderer);

	Sprite sprite1("testSprite", "res/textures/TestTexture.png");
	Sprite sprite2("testSprite", "res/textures/TestTexture.png");
	Sprite background("background", "res/textures/florence.jpg");
	background.setSize(windowWidth, windowHeight);
	background.setCenterPivot();
	sprite1.setCenterPivot();
	sprite1.move(glm::vec2(100.0f, 100.0f));
	sprite2.setCenterPivot();
	sprite2.move(glm::vec2(200.0f, 200.0f));
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

		Input::update();

		if(currentScene)
			currentScene->update(deltaTime);

		sprite2.rotate(0.01f);

		//camera.move(glm::vec2(Input::getAxis("Horizontal"), Input::getAxis("Vertical")) * speed);

		sprite1.move(Input::getAxis("Vertical") * speed * sprite1.forward());
		sprite1.rotate(-Input::getAxis("Horizontal") * 0.03); 

		/////////////////

		///// Draw /////

		currentScene->draw(renderer);

		////////////////

		renderer.swapBuffers();
		renderer.pollEvents();
	}
}

}