#ifndef __GLFW_EXTENSIONS_HPP__
#define __GLFW_EXTENSIONS_HPP__

#define GL_CALL(glFunction)\
    glClearError(); \
	glFunction; \
	assert(glLogCall(#glFunction, __FILE__, __LINE__))

void glClearError();
bool glLogCall(const char* function, const char* file, int line);

#endif // __GLFW_EXTENSIONS_HPP__