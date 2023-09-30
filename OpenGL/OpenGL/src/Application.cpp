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

#include "tests/TestClearColor.h"
#include "tests/TestTexture2D.h"
#include "tests/TestBatchRendering.h"
#include "tests/TestBatchRenderingColors.h"
#include "tests/TestBatchRenderingTextures.h"
#include "tests/TestBatchRenderingDynamic.h"

void render(GLFWwindow* window, int windowWidth, int windowHeight) {
    Renderer renderer;

    ImGui::CreateContext();
    ImGui_ImplGlfwGL3_Init(window, true);
    ImGui::StyleColorsDark();

    test::Test* currentTest = nullptr;
    test::TestMenu* testMenu = new test::TestMenu(currentTest);
    currentTest = testMenu;

    testMenu->registerTest<test::TestClearColor>("Clear Color");
    testMenu->registerTest<test::TestTexture2D>("2D Texture");
    testMenu->registerTest<test::TestBatchRendering>("Batch Rendering");
    testMenu->registerTest<test::TestBatchRenderingColors>("Batch Rendering Colors");
    testMenu->registerTest<test::TestBatchRenderingTextures>("Batch Rendering Textures");
    testMenu->registerTest<test::TestBatchRenderingDynamic>("Batch Rendering Dynamic");
    currentTest = new test::TestBatchRenderingDynamic;

    while (!glfwWindowShouldClose(window))
    {
        GLCall(glClearColor(0.0f, 0.0f, 0.0f, 1.0f))
        renderer.clear();

        ImGui_ImplGlfwGL3_NewFrame();
        if (currentTest) {
            currentTest->onUpdate(0.0f);
            currentTest->onRender();
            ImGui::Begin("Test");
            if (currentTest != testMenu && ImGui::Button("<-")) {
                delete currentTest;
                currentTest = testMenu;
            }
            currentTest->onImGuiRender();
            ImGui::End();
        }

        ImGui::Render();
        ImGui_ImplGlfwGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);

        glfwPollEvents();
    }
    delete currentTest;
    if (currentTest != testMenu) {
        delete testMenu;
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