#ifndef __INDEX_BUFFER_HPP__
#define __INDEX_BUFFER_HPP__

namespace prim
{

class IndexBuffer
{
private:
	unsigned int gl_id;
	unsigned int count;

	void unload();
public:
	IndexBuffer(const unsigned int* data, unsigned int count);
	IndexBuffer(IndexBuffer&& other);
	IndexBuffer& operator=(IndexBuffer&& other);
	~IndexBuffer();

	void bind() const;
	void unbind() const;

	inline unsigned int getCount() const { return count; }
};

}



#endif // __INDEX_BUFFER_HPP__