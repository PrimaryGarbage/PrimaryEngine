#include "vertex_array.hpp"
#include "renderer.hpp"

VertexArray::VertexArray()
{
	GL_CALL(glGenVertexArrays(1, &id)); 
	glBindVertexArray(id);
}

VertexArray::~VertexArray()
{
	GL_CALL(glDeleteVertexArrays(1, &id));
}

void VertexArray::addBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout)
{
	bind();
	vb.bind();
	const std::vector<VertexBufferElement> elements = layout.getElements();
	unsigned int offset = 0;
	for(int i = 0; i < elements.size(); ++i)
	{
		const VertexBufferElement& element = elements[i];
		GL_CALL(glEnableVertexAttribArray(i));
		GL_CALL(glVertexAttribPointer(i, element.count, element.type, element.normalized, layout.getStride(), (const void*)offset));
		offset += element.count * VertexBufferElement::getSizeOfType(element.type);
	}
}

void VertexArray::bind() const
{
	GL_CALL(glBindVertexArray(id));
}

void VertexArray::unbind() const
{
	GL_CALL(glBindVertexArray(0));
}
