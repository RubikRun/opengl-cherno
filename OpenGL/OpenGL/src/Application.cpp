#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>

#include "Renderer.h"
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "Shader.h"
#include "Texture.h"

void render(GLFWwindow* window) {
    float positions[] = {
        -0.5f, -0.5f, 0.0f, 0.0f,
        +0.5f, -0.5f, 1.0f, 0.0f,
        +0.5f, +0.5f, 1.0f, 1.0f,
        -0.5f, +0.5f, 0.0f, 1.0f
    };

    unsigned int indices[2 * 3] = {
        0, 1, 2,
        2, 3, 0
    };

    VertexArray va;
    VertexBuffer vb(positions, 4 * 4 * sizeof(float));

    VertexBufferLayout layout;
    layout.push<float>(2);
    layout.push<float>(2);
    va.addBuffer(vb, layout);

    IndexBuffer ib(indices, 6);

    Shader shader("res/shaders/Basic.shader");
    shader.bind();
    shader.setUniform1i("u_Texture", 0);

    // Unbind everything to demonstrate that we don't have to bind vertex buffer and index buffer,
    // but instead only the vertex array that they are connected to.
    va.unbind();
    shader.unbind();
    vb.unbind();
    ib.unbind();

    Texture texture("res/textures/texture01.png");
    texture.bind();

    Renderer renderer;

    float red = 0.0f;
    float redDelta = 0.005f;
    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        renderer.clear();

        shader.bind();
        shader.setUniform4f("u_Color", red, 0.8f, 0.3f, 1.0f);
        renderer.draw(va, ib, shader);

        /* Update uniforms */
        if (red > 1.0f) {
            redDelta *= -1.0f;
        }
        else if (red < 0.0f) {
            redDelta *= -1.0f;
        }
        red += redDelta;

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }
}

int main(void)
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
    if (!window) {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    glfwSwapInterval(1);

    if (glewInit() != GLEW_OK) {
        std::cout << "Error!" << std::endl;
    }

    std::cout << "OpenGL version: " << glGetString(GL_VERSION) << std::endl;

    // Setup blending function for textures
    GLCall(glEnable(GL_BLEND));
    GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

    render(window);

    glfwTerminate();
    return 0;
}