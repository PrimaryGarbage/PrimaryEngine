#include "renderer.hpp"
#include "logger.hpp"

namespace prim
{

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

void Renderer::draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader) const
{
	va.bind();
	ib.bind();
	shader.bind();
	GL_CALL(glDrawElements(GL_TRIANGLES, ib.getCount(), GL_UNSIGNED_INT, nullptr));
}

void Renderer::clear()
{
	GL_CALL(glClear(GL_COLOR_BUFFER_BIT));
}

}

