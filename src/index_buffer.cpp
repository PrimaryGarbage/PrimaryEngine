#include "index_buffer.hpp"
#include "renderer.hpp"


namespace prim
{

IndexBuffer::IndexBuffer(const unsigned int* data, unsigned int count) : count(count)
{
	GL_CALL(glGenBuffers(1, &gl_id));
	GL_CALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gl_id));
	GL_CALL(glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), data, GL_STATIC_DRAW));
}

IndexBuffer::IndexBuffer(IndexBuffer&& other)
{
	gl_id = other.gl_id;
	other.gl_id = 0;
}

IndexBuffer::~IndexBuffer()
{
	GL_CALL(glDeleteBuffers(1, &gl_id));
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

