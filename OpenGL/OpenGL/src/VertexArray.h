#pragma once

#include "VertexBuffer.h"

class VertexBufferLayout;

class VertexArray
{
private:
	unsigned int rendererId;
public:
	VertexArray();
	~VertexArray();

	void addBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout);

	void bind() const;
	void unbind() const;
};