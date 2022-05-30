#ifndef __INDEX_BUFFER_HPP__
#define __INDEX_BUFFER_HPP__

class IndexBuffer
{
private:
	unsigned int id;
	unsigned int count;
public:
	IndexBuffer(const unsigned int* data, unsigned int count);
	~IndexBuffer();

	void Bind() const;
	void Unbind() const;

	inline unsigned int getCount() const { return count; }
};

#endif // __INDEX_BUFFER_HPP__