#include "vertex_array.hpp"
#include "renderer.hpp"

namespace prim
{

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

VertexArray::~VertexArray()
{
	GL_CALL(glDeleteVertexArrays(1, &gl_id));
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
	GL_CALL(glBindVertexArray(gl_id));
}

void VertexArray::unbind() const
{
	GL_CALL(glBindVertexArray(0));
}

}


