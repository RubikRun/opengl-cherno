#include "VertexBuffer.h"

#include "Renderer.h"

VertexBuffer::VertexBuffer(const void* data, unsigned int size, int drawType) {
    GLCall(glGenBuffers(1, &rendererId));
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, rendererId));
    GLCall(glBufferData(GL_ARRAY_BUFFER, size, data, drawType));
}

VertexBuffer::~VertexBuffer() {
    GLCall(glDeleteBuffers(1, &rendererId));
}

void VertexBuffer::bind() const {
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, rendererId));
}

void VertexBuffer::unbind() const {
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
}