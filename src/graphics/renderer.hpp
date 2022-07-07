#ifndef __RENDERER_HPP__
#define __RENDERER_HPP__

#include <cassert>
#include "GL/glew.h"
#include "GLFW/glfw3.h"
#include "mesh.hpp"

namespace prim
{

#define GL_CALL(glFunction) glClearError(); \
	glFunction; \
	assert(glLogCall(#glFunction, __FILE__, __LINE__))

void glClearError();
bool glLogCall(const char* function, const char* file, int line);

class Renderer
{
private:
	const unsigned int defaultWidth = 800u;
	const unsigned int defaultHeight = 800u;
	const char* defaultName = "Prim Engine";

	GLFWwindow* window = nullptr;
	std::vector<Mesh*> drawList;
	int currentShaderId = 0;

	static void framebuffer_size_callback(GLFWwindow* window, int width, int height);
	static void error_callback(int error, const char* description);

public:
	Renderer() = default;
	~Renderer();

	void init(unsigned int windowWidth, unsigned int windowHeight, const char* windowName);
	void drawLists() const;
	void drawMesh(const Mesh& mesh);
	void addMesh(Mesh* mesh);
	void clear();
	bool windowShouldClose();
	void swapBuffers();
	void pollEvents();
	const std::vector<Mesh*>&  getDrawList() const;
};

}


#endif // __RENDERER_HPP__