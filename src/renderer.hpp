#ifndef __RENDERER_HPP__
#define __RENDERER_HPP__

#include <cassert>
#include "GL/glew.h"
#include "index_buffer.hpp"
#include "vertex_array.hpp"
#include "shader.hpp"

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

public:
	void draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader) const;
	void clear();
};

}


#endif // __RENDERER_HPP__