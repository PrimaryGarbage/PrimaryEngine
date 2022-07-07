#ifndef __VERTEX_ARRAY_HPP__
#define __VERTEX_ARRAY_HPP__

#include "vertex_buffer.hpp"
#include "vertex_buffer_layout.hpp"

namespace prim
{

class VertexArray
{
private:
	unsigned int gl_id;
public:
	VertexArray();
	VertexArray(VertexArray&& other);
	VertexArray& operator=(VertexArray&& other);
	~VertexArray();

	void addBuffer(const VertexBuffer& vb);

	void bind() const;
	void unbind() const;
};

}



#endif // __VERTEX_ARRAY_HPP__