#ifndef __VERTEX_BUFFER_HPP__
#define __VERTEX_BUFFER_HPP__

namespace prim
{

class VertexBuffer
{
private:
	unsigned int gl_id;
public:
	VertexBuffer(const void* data, unsigned int size);
	VertexBuffer(VertexBuffer&& other);
	~VertexBuffer();

	void bind() const;
	void unbind() const;
};

}



#endif // __VERTEX_BUFFER_HPP__