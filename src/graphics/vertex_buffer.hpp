#ifndef __VERTEX_BUFFER_HPP__
#define __VERTEX_BUFFER_HPP__

#include "vertex_buffer_layout.hpp"

namespace prim
{

class VertexBuffer
{
private:
	unsigned int gl_id;
	VertexBufferLayout layout;

	void unload();
public:
	VertexBuffer(const void* data, unsigned int size, VertexBufferLayout layout);
	VertexBuffer(VertexBuffer&& other);
	VertexBuffer& operator=(VertexBuffer&& other);
	~VertexBuffer();

	void bind() const;
	void unbind() const;
	inline const VertexBufferLayout getLayout() const { return layout; }
};

}



#endif // __VERTEX_BUFFER_HPP__