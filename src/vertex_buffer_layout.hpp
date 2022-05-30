#ifndef __VERTEX_BUFFER_LAYOUT_HPP__
#define __VERTEX_BUFFER_LAYOUT_HPP__

#include <vector>
#include "GL/glew.h"

struct VertexBufferElement
{
	unsigned int type;
	unsigned int count;
	unsigned char normalized;

	static unsigned int getSizeOfType(unsigned int type)
	{
		switch(type)
		{
			case GL_FLOAT: 			return 4;
			case GL_INT:			return 4;
			case GL_UNSIGNED_BYTE: 	return 1;
		}
		return 0;
	}
};

class VertexBufferLayout
{
private:
	std::vector<VertexBufferElement> elements;
	unsigned int stride;
public:
	VertexBufferLayout() : stride(0) {}

	template<class T>
	void push(int count)
	{
		static_assert(false);
	}

	template<>
	void push<float>(unsigned int count)
	{
		elements.push_back({ GL_FLOAT, count, GL_FALSE });
		stride += VertexBufferElement::getSizeOfType(GL_FLOAT) * count;
	}

	template<>
	void push<unsigned int>(unsigned int count)
	{
		elements.push_back({ GL_UNSIGNED_INT, count, GL_FALSE });
		stride += VertexBufferElement::getSizeOfType(GL_UNSIGNED_INT) * count;
	}

	template<>
	void push<unsigned char>(unsigned int count)
	{
		elements.push_back({ GL_UNSIGNED_BYTE, count, GL_TRUE });
		stride += sizeof(GLubyte) * count;
		stride += VertexBufferElement::getSizeOfType(GL_UNSIGNED_BYTE) * count;
	}

	inline const std::vector<VertexBufferElement>& getElements() const { return elements; }
	inline unsigned int getStride() const { return stride; }
};

#endif // __VERTEX_BUFFER_LAYOUT_HPP__