#ifndef __VERTEX_ARRAY_HPP__
#define __VERTEX_ARRAY_HPP__

#include "vertex_buffer.hpp"
#include "vertex_buffer_layout.hpp"

class VertexArray
{
private:
	unsigned int id;
public:
	VertexArray();
	~VertexArray();

	void addBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout);

	void bind() const;
	void unbind() const;
};

#endif // __VERTEX_ARRAY_HPP__