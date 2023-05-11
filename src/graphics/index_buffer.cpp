#include "index_buffer.hpp"
#include "glfw_extensions.hpp"
#include "GL/glew.h"
#include "GLFW/glfw3.h"
#include <cassert>


namespace prim
{
	void IndexBuffer::unload()
	{
		if (gl_id > 0)
		{
			unbind();
			GL_CALL(glDeleteBuffers(1, &gl_id));
			gl_id = 0;
		}
	}


	IndexBuffer::IndexBuffer(const unsigned int* data, unsigned int count) : count(count)
	{
		GL_CALL(glGenBuffers(1, &gl_id));
		GL_CALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gl_id));
		GL_CALL(glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), data, GL_STATIC_DRAW));
	}

	IndexBuffer::IndexBuffer(IndexBuffer&& other) :
		gl_id(other.gl_id), count(other.count)
	{
		other.gl_id = 0;
		other.count = 0;
	}

	IndexBuffer& IndexBuffer::operator=(IndexBuffer&& other)
	{
		unload();

		gl_id = other.gl_id;
		count = other.count;

		other.gl_id = 0;
		other.count = 0;
		return *this;
	}

	IndexBuffer::~IndexBuffer()
	{
		unload();
	}

	void IndexBuffer::bind() const
	{
		GL_CALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gl_id));
	}

	void IndexBuffer::unbind() const
	{
		GL_CALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
	}

}

