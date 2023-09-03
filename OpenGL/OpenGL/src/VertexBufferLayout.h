#pragma once

#include <vector>
#include <GL/glew.h>
#include "Renderer.h"

struct VertexBufferElement
{
	unsigned int type;
	unsigned int count;
	unsigned char normalized;

	static unsigned int getSizeOfType(unsigned int type) {
		switch (type) {
			case GL_FLOAT: return sizeof(GLfloat);
			case GL_UNSIGNED_INT: return sizeof(GLuint);
			case GL_UNSIGNED_BYTE: return sizeof(GLubyte);
		}
		ASSERT(false);
		return 0;
	}
};

class VertexBufferLayout
{
private:
	std::vector<VertexBufferElement> elements;
	unsigned int stride;

public:
	VertexBufferLayout()
		: stride(0) {}

	template <typename T>
	void push(unsigned int count) {
		ASSERT(false);
	}

	template <>
	void push<float>(unsigned int count) {
		VertexBufferElement element = { GL_FLOAT, count, GL_FALSE };
		elements.push_back(element);
		stride += VertexBufferElement::getSizeOfType(GL_FLOAT) * count;
	}

	template<>
	void push<unsigned int>(unsigned int count) {
		VertexBufferElement element = { GL_UNSIGNED_INT, count, GL_FALSE };
		elements.push_back(element);
		stride += VertexBufferElement::getSizeOfType(GL_UNSIGNED_INT) * count;
	}

	template<>
	void push<unsigned char>(unsigned int count) {
		VertexBufferElement element = { GL_UNSIGNED_BYTE, count, GL_TRUE };
		elements.push_back(element);
		stride += VertexBufferElement::getSizeOfType(GL_UNSIGNED_BYTE) * count;
	}

	inline const std::vector<VertexBufferElement>& getElements() const { return elements; }
	inline unsigned int getStride() const { return stride; }
};