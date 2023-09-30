#pragma once

class VertexBuffer
{
private:
	unsigned int rendererId;
public:
	VertexBuffer(const void* data, unsigned int size, int drawType);
	~VertexBuffer();

	void bind() const;
	void unbind() const;
};