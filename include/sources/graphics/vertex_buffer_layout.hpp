#ifndef __VERTEX_BUFFER_LAYOUT_HPP__
#define __VERTEX_BUFFER_LAYOUT_HPP__

#include <vector>
#include <cassert>
#include "glew/GL/glew.h"

namespace prim
{

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
	void push(unsigned int count, bool normalized = false)
	{
		assert(false);
	}

	void bind() const;

	inline const std::vector<VertexBufferElement>& getElements() const { return elements; }
	inline unsigned int getStride() const { return stride; }
};

template<>
inline void VertexBufferLayout::push<float>(unsigned int count, bool normalized)
{
	elements.push_back({ GL_FLOAT, count, (unsigned char)normalized });
	stride += VertexBufferElement::getSizeOfType(GL_FLOAT) * count;
}

template<>
inline void VertexBufferLayout::push<unsigned int>(unsigned int count, bool normalized)
{
	elements.push_back({ GL_UNSIGNED_INT, count, (unsigned char)normalized });
	stride += VertexBufferElement::getSizeOfType(GL_UNSIGNED_INT) * count;
}

template<>
inline void VertexBufferLayout::push<unsigned char>(unsigned int count, bool normalized)
{
	elements.push_back({ GL_UNSIGNED_BYTE, count, (unsigned char)normalized });
	stride += sizeof(GLubyte) * count;
	stride += VertexBufferElement::getSizeOfType(GL_UNSIGNED_BYTE) * count;
}

}



#endif // __VERTEX_BUFFER_LAYOUT_HPP__