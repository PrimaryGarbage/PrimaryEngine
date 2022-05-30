#include "renderer.hpp"
#include "logger.hpp"

void glClearError()
{
	while(glGetError() != GL_NO_ERROR);
}

bool glLogCall(const char* function, const char* file, int line)
{
	if(GLenum error = glGetError())
	{
		Logger::log("[OpenGL Error] (" + std::to_string(error) + "): " + function + " " + file + ":" + std::to_string(line), true);
		return false;
	}
	return true;
}