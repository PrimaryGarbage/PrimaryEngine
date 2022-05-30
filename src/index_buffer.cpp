#include "index_buffer.hpp"
#include "renderer.hpp"

IndexBuffer::IndexBuffer(const unsigned int* data, unsigned int count) : count(count)
{
	GL_CALL(glGenBuffers(1, &id));
	GL_CALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id));
	GL_CALL(glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), data, GL_STATIC_DRAW));
}

IndexBuffer::~IndexBuffer()
{
	GL_CALL(glDeleteBuffers(1, &id));
}

void IndexBuffer::Bind() const
{
	GL_CALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id));
}

void IndexBuffer::Unbind() const
{
	GL_CALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
}