#ifndef __RENDERER_HPP__
#define __RENDERER_HPP__

#include <cassert>
#include "GL/glew.h"
#include "GLFW/glfw3.h"
#include "mesh.hpp"
#include <unordered_map>
#include "camera_base.hpp"

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
	inline static std::unordered_map<GLFWwindow*, Renderer*> windowRendererMap;

	const unsigned int defaultWidth = 800u;
	const unsigned int defaultHeight = 800u;
	const char* defaultName = "Prim Engine";

	GLFWwindow* window = nullptr;
	int windowWidth;
	int windowHeight;
	std::vector<Mesh*> drawList;
	mutable unsigned int currentShaderId = 0;
	glm::mat4 projectMat;
	glm::mat4 viewMat;
	glm::mat4 modelMat;
	bool preparedForDrawing = false;
	CameraBase* currentCamera = nullptr;

	static void framebuffer_size_callback(GLFWwindow* window, int width, int height);
	static void error_callback(int error, const char* description);

	void prepareForDrawing();

public:
	Renderer() = default;
	~Renderer();

	void init(unsigned int windowWidth, unsigned int windowHeight, const char* windowName);
	void drawLists();
	void drawMesh(const Mesh& mesh);
	void addMesh(Mesh* mesh);
	void clear();
	bool windowShouldClose();
	void swapBuffers();
	void pollEvents();
	const std::vector<Mesh*>&  getDrawList() const;
	inline GLFWwindow* getWindow() const { return window; }
	inline void setProjectMat(glm::mat4 proj) { projectMat = proj; }
	inline void setViewMat(glm::mat4 view) { viewMat = view; }
	inline void setModelMat(glm::mat4 model) { modelMat = model; }
	inline int getWindowWidth() const { return windowWidth; }
	inline int getWindowHeight() const { return windowHeight; }
	inline void setCurrentCamera(CameraBase* camera){ currentCamera = camera; }
	inline CameraBase* getCurrentCamera() const { return currentCamera; }
};

}


#endif // __RENDERER_HPP__