#ifndef __RENDERER_HPP__
#define __RENDERER_HPP__

#include <cassert>
#include "GL/glew.h"
#include "vertex_buffer.hpp"
#include "index_buffer.hpp"

#define GL_CALL(glFunction) glClearError(); \
	glFunction; \
	assert(glLogCall(#glFunction, __FILE__, __LINE__))

void glClearError();
bool glLogCall(const char* function, const char* file, int line);


#endif // __RENDERER_HPP__