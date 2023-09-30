#include "Renderer.h"

#include <iostream>

void GLClearError() {
    while (glGetError() != GL_NO_ERROR);
}

bool GLLogCall(const char* function, const char* file, int line) {
    if (GLenum error = glGetError()) {
        std::cout << "[OpenGL Error] (" << error << "): "
            << function << " " << file << ":" << line << std::endl;
        return false;
    }
    return true;
}

void Renderer::draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader, int indicesToDraw) const {
    shader.bind();
    va.bind();
    ib.bind();

    const int idxCount = (indicesToDraw == -1) ? ib.getCount() : indicesToDraw;
    ASSERT(idxCount > 0 && idxCount < ib.getCount());
    GLCall(glDrawElements(GL_TRIANGLES, idxCount, GL_UNSIGNED_INT, nullptr));
}

void Renderer::clear() const {
    GLCall(glClear(GL_COLOR_BUFFER_BIT));
}