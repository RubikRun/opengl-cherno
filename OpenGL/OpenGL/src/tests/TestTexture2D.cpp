#include "TestTexture2D.h"

#include "Renderer.h"
#include "imgui/imgui.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

namespace test {

	TestTexture2D::TestTexture2D()
        : windowWidth(920.0f)
        , windowHeight(560.0f)
        , proj(glm::ortho(0.0f, float(windowWidth), 0.0f, float(windowHeight), -1.0f, 1.0f))
        , view(glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0)))
        , translationA(200, 200, 0)
        , translationB(400, 200, 0)
	{
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

        shader = std::make_unique<Shader>("res/shaders/Basic.shader");
        vertexArray = std::make_unique<VertexArray>();
        vertexBuffer = std::make_unique<VertexBuffer>(positions, 4 * 4 * sizeof(float));

        VertexBufferLayout layout;
        layout.push<float>(2);
        layout.push<float>(2);
        vertexArray->addBuffer(*vertexBuffer, layout);

        indexBuffer = std::make_unique<IndexBuffer>(indices, 6);

        shader->bind();
        shader->setUniform1i("u_Texture", 0);

        texture = std::make_unique<Texture>("res/textures/spongebob.png");
	}

	TestTexture2D::~TestTexture2D() {}

	void TestTexture2D::onUpdate(float deltaTime) {}

	void TestTexture2D::onRender() {
		GLCall(glClearColor(0.0f, 0.0f, 0.0f, 1.0f));
		GLCall(glClear(GL_COLOR_BUFFER_BIT));

        Renderer renderer;

        texture->bind();

        {
            glm::mat4 model = glm::translate(glm::mat4(1.0f), translationA);
            glm::mat4 mvp = proj * view * model;
            shader->bind();
            shader->setUniformMat4f("u_MVP", mvp);

            renderer.draw(*vertexArray, *indexBuffer, *shader);
        }
        {
            glm::mat4 model = glm::translate(glm::mat4(1.0f), translationB);
            glm::mat4 mvp = proj * view * model;
            shader->bind();
            shader->setUniformMat4f("u_MVP", mvp);

            renderer.draw(*vertexArray, *indexBuffer, *shader);
        }
	}

	void TestTexture2D::onImGuiRender() {
        ImGui::SliderFloat3("Translation A", &translationA.x, 0.0f, float(windowWidth));
        ImGui::SliderFloat3("Translation B", &translationB.x, 0.0f, float(windowWidth));
        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	}

} // namespace test