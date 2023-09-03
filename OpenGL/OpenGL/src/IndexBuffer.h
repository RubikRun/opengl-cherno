#pragma once

class IndexBuffer
{
private:
	unsigned int rendererId;
	unsigned int count;
public:
	IndexBuffer(const unsigned* data, unsigned int count);
	~IndexBuffer();

	void bind() const;
	void unbind() const;

	inline unsigned int getCount() const { return count; };
};