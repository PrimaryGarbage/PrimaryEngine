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

void PrimaryApp::mainLoop()
{
	Camera2D camera("Player camera", &renderer);

	Sprite sprite1("testSprite", "res/textures/TestTexture.png");
	Sprite sprite2("testSprite", "res/textures/TestTexture.png");
	sprite1.addChild(&sprite2);
	sprite2.addChild(&camera);
	sprite2.move(glm::vec2(200.0f, 40.0f));
	float speed = 10.0f;


	while(!renderer.windowShouldClose())
	{
		deltaTime = timer.peekSinceLastPeek() * 0.001f;
		timeSinceStart = timer.peek() * 0.001f;

		renderer.clear();

		///// Update /////

		Input::update();

		sprite2.move(glm::vec2(Input::getAxis("Horizontal") * speed, Input::getAxis("Vertical") * speed));

		if(Input::isPressed(Key::comma)) sprite2.scale(0.99f);
		if(Input::isPressed(Key::period)) sprite2.scale(1.01f);

		sprite2.rotate(0.01f);

		/////////////////

		for(const Mesh* mesh : renderer.getDrawList())
			for(const MeshComposition& composition : mesh->compositions)
				composition.shader.setUniform1f("u_time", timeSinceStart);

		///// Draw /////

		sprite1.draw(renderer);
		sprite2.draw(renderer);

		renderer.drawLists();
		
		////////////////

		renderer.swapBuffers();
		renderer.pollEvents();
	}
}

}