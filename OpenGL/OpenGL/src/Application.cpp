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

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw_gl3.h"

void render(GLFWwindow* window, int windowWidth, int windowHeight) {
    const float xCenter = 0.0f;
    const float yCenter = 0.0f;
    const float squareRadius = float(windowWidth) / 4.0f;
    float positions[] = {
        xCenter - squareRadius, yCenter - squareRadius, 0.0f, 0.0f,
        xCenter + squareRadius, yCenter - squareRadius, 1.0f, 0.0f,
        xCenter + squareRadius, yCenter + squareRadius, 1.0f, 1.0f,
        xCenter - squareRadius, yCenter + squareRadius, 0.0f, 1.0f
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

    glm::mat4 proj = glm::ortho(0.0f, float(windowWidth), 0.0f, float(windowHeight), - 1.0f, 1.0f);
    glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0));

    Shader shader("res/shaders/Basic.shader");
    shader.bind();
    shader.setUniform1i("u_Texture", 0);

    // Unbind everything to demonstrate that we don't have to bind vertex buffer and index buffer,
    // but instead only the vertex array that they are connected to.
    va.unbind();
    shader.unbind();
    vb.unbind();
    ib.unbind();

    Texture texture("res/textures/spongebob.png");
    texture.bind();

    Renderer renderer;

    ImGui::CreateContext();
    ImGui_ImplGlfwGL3_Init(window, true);
    ImGui::StyleColorsDark();

    glm::vec3 translationA(200, 200, 0);
    glm::vec3 translationB(400, 200, 0);

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        renderer.clear();

        ImGui_ImplGlfwGL3_NewFrame();

        {
            glm::mat4 model = glm::translate(glm::mat4(1.0f), translationA);
            glm::mat4 mvp = proj * view * model;
            shader.bind();
            shader.setUniformMat4f("u_MVP", mvp);

            renderer.draw(va, ib, shader);
        }
        {
            glm::mat4 model = glm::translate(glm::mat4(1.0f), translationB);
            glm::mat4 mvp = proj * view * model;
            shader.bind();
            shader.setUniformMat4f("u_MVP", mvp);

            renderer.draw(va, ib, shader);
        }

        {
            ImGui::SliderFloat3("Translation A", &translationA.x, 0.0f, float(windowWidth));
            ImGui::SliderFloat3("Translation B", &translationB.x, 0.0f, float(windowWidth));
            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
        }

        ImGui::Render();
        ImGui_ImplGlfwGL3_RenderDrawData(ImGui::GetDrawData());

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

    const int windowWidth = 920;
    const int windowHeight = 560;

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(windowWidth, windowHeight, "Hello World", NULL, NULL);
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

    render(window, windowWidth, windowHeight);

    ImGui_ImplGlfwGL3_Shutdown();
    ImGui::DestroyContext();
    glfwTerminate();
    return 0;
}