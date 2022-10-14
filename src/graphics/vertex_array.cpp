#include "vertex_array.hpp"
#include "renderer.hpp"

namespace prim
{
	void VertexArray::unload()
	{
		if (gl_id > 0)
		{
			unbind();
			GL_CALL(glDeleteVertexArrays(1, &gl_id));
			gl_id = 0;
		}
	}


	VertexArray::VertexArray()
	{
		GL_CALL(glGenVertexArrays(1, &gl_id));
		glBindVertexArray(gl_id);
	}

	VertexArray::VertexArray(VertexArray&& other)
	{
		gl_id = other.gl_id;
		other.gl_id = 0;
	}

	VertexArray& VertexArray::operator=(VertexArray&& other)
	{
		unload();

		gl_id = other.gl_id;
		other.gl_id = 0;

		return *this;
	}

	VertexArray::~VertexArray()
	{
		unload();
	}

	void VertexArray::addBuffer(const VertexBuffer& vb)
	{
		bind();
		vb.bind();
		vb.getLayout().bind();
	}

	void VertexArray::bind() const
	{
		if (gl_id > 0)
			GL_CALL(glBindVertexArray(gl_id));
	}

	void VertexArray::unbind() const
	{
		GL_CALL(glBindVertexArray(0));
	}

}


