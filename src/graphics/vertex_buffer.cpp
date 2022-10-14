#include "vertex_buffer.hpp"
#include "renderer.hpp"

namespace prim
{

VertexBuffer::VertexBuffer(const void* data, unsigned int size, VertexBufferLayout layout) : layout(layout)
{
	GL_CALL(glGenBuffers(1, &gl_id));
	GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, gl_id));
	GL_CALL(glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW));
}

VertexBuffer::VertexBuffer(VertexBuffer&& other)
	: gl_id(other.gl_id), layout(other.layout)
{
	other.gl_id = 0;
}

VertexBuffer& VertexBuffer::operator=(VertexBuffer&& other)
{
	unload();

	gl_id = other.gl_id;
	layout = other.layout;

	other.gl_id = 0;
	return *this;
}

VertexBuffer::~VertexBuffer()
{
	unload();
}

void VertexBuffer::bind() const
{
	GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, gl_id));
	layout.bind();
}

void VertexBuffer::unbind() const
{
	GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, 0));
}

void VertexBuffer::unload()
{
	if(gl_id > 0)
	{
		unbind();
		GL_CALL(glDeleteBuffers(1, &gl_id));
		gl_id = 0;
	}
}

}


