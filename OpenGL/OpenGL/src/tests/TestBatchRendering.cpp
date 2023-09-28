#include "TestBatchRendering.h"

#include "Renderer.h"
#include "imgui/imgui.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

namespace test {

    TestBatchRendering::TestBatchRendering()
        : windowWidth(920.0f)
        , windowHeight(560.0f)
        , proj(glm::ortho(0.0f, float(windowWidth), 0.0f, float(windowHeight), -1.0f, 1.0f))
        , view(glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0)))
	{
        const float xCenterA = float(windowWidth) * 2.0f / 5.0f;
        const float yCenterA = float(windowHeight) * 2.0f / 5.0f;
        const float squareRadiusA = float(windowWidth) / 6.0f;
        const float xCenterB = float(windowWidth) * 3.0f / 5.0f;
        const float yCenterB = float(windowHeight) * 3.0f / 5.0f;
        const float squareRadiusB = float(windowWidth) / 8.0f;
        float positions[] = {
            xCenterA - squareRadiusA, yCenterA - squareRadiusA,
            xCenterA + squareRadiusA, yCenterA - squareRadiusA,
            xCenterA + squareRadiusA, yCenterA + squareRadiusA,
            xCenterA - squareRadiusA, yCenterA + squareRadiusA,
            xCenterB - squareRadiusB, yCenterB - squareRadiusB,
            xCenterB + squareRadiusB, yCenterB - squareRadiusB,
            xCenterB + squareRadiusB, yCenterB + squareRadiusB,
            xCenterB - squareRadiusB, yCenterB + squareRadiusB
        };
        unsigned int indices[2 * 3 * 2] = {
            0, 1, 2,
            2, 3, 0,
            4, 5, 6,
            6, 7, 4
        };

        shader = std::make_unique<Shader>("res/shaders/BatchRendering.shader");
        vertexArray = std::make_unique<VertexArray>();
        vertexBuffer = std::make_unique<VertexBuffer>(positions, 8 * 2 * sizeof(float));

        VertexBufferLayout layout;
        layout.push<float>(2);
        vertexArray->addBuffer(*vertexBuffer, layout);

        indexBuffer = std::make_unique<IndexBuffer>(indices, 2 * 3 * 2);

        shader->bind();
	}

    TestBatchRendering::~TestBatchRendering() {}

	void TestBatchRendering::onUpdate(float deltaTime) {}

	void TestBatchRendering::onRender() {
		GLCall(glClearColor(0.0f, 0.0f, 0.0f, 1.0f));
		GLCall(glClear(GL_COLOR_BUFFER_BIT));

        Renderer renderer;

        glm::vec3 translation(0, 0, 0);
        glm::mat4 model = glm::translate(glm::mat4(1.0f), translation);
        glm::mat4 mvp = proj * view * model;
        shader->bind();
        shader->setUniformMat4f("u_MVP", mvp);

        renderer.draw(*vertexArray, *indexBuffer, *shader);
	}

	void TestBatchRendering::onImGuiRender() {
        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	}

} // namespace test