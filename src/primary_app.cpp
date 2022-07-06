#include "primary_app.hpp"
#include "logger.hpp"
#include "prim_exception.hpp"
#include <stdexcept>
#include <cassert>
#include <filesystem>
#include "glm.hpp"
#include "gtc/matrix_transform.hpp"

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
}

void PrimaryApp::run()
{
	//// TEMP ////

	float positions[] = {
		100.0f, 100.0f, 0.0f, 0.0f,
		200.0f, 100.0f, 1.0f, 0.0f,
		200.0f, 200.0f, 1.0f, 1.0f,
		100.0f, 200.0f, 0.0f, 1.0f,
	};

	unsigned int indices[] = {
		0, 1, 2,
		0, 2, 3
	};

	VertexBufferLayout layout;
	layout.push<float>(2);
	layout.push<float>(2);

	VertexBuffer vb(positions, 16 * sizeof(float), layout);

	glm::mat4 proj = glm::ortho(0.0f, static_cast<float>(windowWidth), 0.0f, static_cast<float>(windowHeight), -1.0f, 1.0f);
	glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(-100, 0, 0));
	glm::mat4 model(1.0f);

	glm::mat4 mvp = proj * view * model;


	Shader shader("res/shaders/default.shader");
	shader.bind();
	shader.setUniform4f("u_color", 0.8f, 0.6f, 0.4f, 1.0f);
	shader.setUniformMat4f("u_mvp", mvp);

	Texture texture("res/textures/TestTexture.png");
	texture.bind();

	shader.setUniform1i("u_texture", 0);

	IndexBuffer ib(indices, 6);

	Mesh mesh(std::move(vb));
	MeshComposition meshComposition(std::move(ib), std::move(shader), std::move(texture));
	mesh.addComposition(std::move(meshComposition));

	renderer.addMesh(&mesh);

	//////////////////

	timer.start();

	mainLoop();
}

void PrimaryApp::mainLoop()
{
	while(!renderer.windowShouldClose())
	{
		deltaTime = timer.peekSinceLastPeek() * 0.001f;
		timeSinceStart = timer.peek() * 0.001f;

		for(const Mesh* mesh : renderer.getDrawList())
			for(const MeshComposition& composition : mesh->compositions)
				composition.shader.setUniform1f("u_time", timeSinceStart);


		renderer.clear();

		///// Draw /////

		renderer.drawLists();

		//renderer.drawModel(*renderer.model);
		
		////////////////

		renderer.swapBuffers();
		renderer.pollEvents();
	}
}


}

