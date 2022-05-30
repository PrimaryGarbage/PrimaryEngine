#ifndef __VERTEX_BUFFER_HPP__
#define __VERTEX_BUFFER_HPP__

class VertexBuffer
{
private:
	unsigned int id;
public:
	VertexBuffer(const void* data, unsigned int size);
	~VertexBuffer();

	void bind() const;
	void unbind() const;
};

#endif // __VERTEX_BUFFER_HPP__