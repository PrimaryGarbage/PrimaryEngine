#include "primary_app.hpp"
#include "logger.hpp"
#include "prim_exception.hpp"
#include <stdexcept>
#include <cassert>
#include <filesystem>

namespace prim
{

PrimaryApp::PrimaryApp(const char* appPath) : appPath(appPath)
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
		-0.5f, -0.5f, 0.0f, 0.0f,
		0.5f, -0.5f, 1.0f, 0.0f,
		0.5f, 0.5f, 1.0f, 1.0f,
		-0.5f, 0.5f, 0.0f, 1.0f,
	};

	unsigned int indices[] = {
		0, 1, 2,
		0, 2, 3
	};

	VertexBufferLayout layout;
	layout.push<float>(2);
	layout.push<float>(2);

	VertexBuffer vb(positions, 16 * sizeof(float), layout);

	Shader shader("res/shaders/default.shader");
	shader.bind();
	shader.setUniform4f("u_color", 0.8f, 0.6f, 0.4f, 1.0f);

	Texture texture("res/textures/TestTexture.png");
	texture.bind();

	shader.setUniform1i("u_texture", 0);

	IndexBuffer ib(indices, 6);

	Mesh mesh(std::move(vb));
	MeshComposition meshComposition(std::move(ib), std::move(shader), std::move(texture));
	mesh.addComposition(std::move(meshComposition));

	Model model;
	model.meshes.push_back(std::move(mesh));

	renderer.addModel(&model);

	//////////////////

	mainLoop();
}

void PrimaryApp::mainLoop()
{
	while(!renderer.windowShouldClose())
	{
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

