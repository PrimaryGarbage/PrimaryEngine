#include "glfw_extensions.hpp"
#include "GL/glew.h"
#include "GLFW/glfw3.h"
#include "globals.hpp"

void glClearError()
{
	while (glGetError() != GL_NO_ERROR)
		;
}

bool glLogCall(const char* function, const char* file, int line)
{
	if (GLenum error = glGetError())
	{
		prim::Logger::inst().logError("[OpenGL Error] (" + std::to_string(error) + "): " + function + " " + file + ":" + std::to_string(line), true);
		return false;
		long long a;
	}
	return true;
}