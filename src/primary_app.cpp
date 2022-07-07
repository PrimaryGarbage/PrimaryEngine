#include "primary_app.hpp"
#include "logger.hpp"
#include "prim_exception.hpp"
#include "input.hpp"
#include <stdexcept>
#include <cassert>
#include <filesystem>
#include "glm.hpp"
#include "gtc/matrix_transform.hpp"
#include "node2d.hpp"
#include "primitives.hpp"
#include "sprite.hpp"

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
	glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(100, 0, 0));

	renderer.setProjectMat(std::move(proj));
	renderer.setViewMat(std::move(view));

	//////////////////

	timer.start();

	mainLoop();
}

void PrimaryApp::mainLoop()
{
	Sprite sprite1("testSprite", "res/textures/TestTexture.png");
	Sprite sprite2("testSprite", "res/textures/TestTexture.png");
	sprite1.addChild(&sprite2);
	sprite2.move(glm::vec2(200.0f, 0.0f));
	float speed = 10.0f;

	while(!renderer.windowShouldClose())
	{
		deltaTime = timer.peekSinceLastPeek() * 0.001f;
		timeSinceStart = timer.peek() * 0.001f;

		for(const Mesh* mesh : renderer.getDrawList())
			for(const MeshComposition& composition : mesh->compositions)
				composition.shader.setUniform1f("u_time", timeSinceStart);


		renderer.clear();

		///// Draw /////

		sprite1.move(glm::vec2(Input::getAxis("Horizontal") * speed, Input::getAxis("Vertical") * speed));

		sprite1.draw(renderer);
		sprite2.draw(renderer);

		renderer.drawLists();

		//renderer.drawModel(*renderer.model);
		
		////////////////

		renderer.swapBuffers();
		renderer.pollEvents();
	}
}

}