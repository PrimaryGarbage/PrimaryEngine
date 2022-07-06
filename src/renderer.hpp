#ifndef __RENDERER_HPP__
#define __RENDERER_HPP__

#include <cassert>
#include "GL/glew.h"
#include "model.hpp"

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
	std::vector<Model*> modelDrawList;

public:
	void drawLists() const;
	void drawModel(const Model& model);
	void addModel(Model* model);
	void clear();
};

}


#endif // __RENDERER_HPP__