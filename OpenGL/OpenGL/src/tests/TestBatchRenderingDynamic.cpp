#include "TestBatchRenderingDynamic.h"

#include "Renderer.h"
#include "imgui/imgui.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include <array>

struct Vertex
{
    float position[2];
    float texCoords[2];
    float texId;
};

namespace test {

    static Vertex* createQuad(Vertex* target, float x, float y, float size, float texId)
    {
        target->position[0] = x; target->position[1] = y;
        target->texCoords[0] = 0.0f; target->texCoords[1] = 0.0f;
        target->texId = texId;
        target++;

        target->position[0] = x + size; target->position[1] = y;
        target->texCoords[0] = 1.0f; target->texCoords[1] = 0.0f;
        target->texId = texId;
        target++;

        target->position[0] = x + size; target->position[1] = y + size;
        target->texCoords[0] = 1.0f; target->texCoords[1] = 1.0f;
        target->texId = texId;
        target++;

        target->position[0] = x; target->position[1] = y + size;
        target->texCoords[0] = 0.0f; target->texCoords[1] = 1.0f;
        target->texId = texId;
        target++;

        return target;
    }

    TestBatchRenderingDynamic::TestBatchRenderingDynamic()
        : windowWidth(920.0f)
        , windowHeight(560.0f)
        , proj(glm::ortho(0.0f, float(windowWidth), 0.0f, float(windowHeight), -1.0f, 1.0f))
        , view(glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0)))
	{
        const size_t maxQuadCount = 1000;
        const size_t maxVertexCount = maxQuadCount * 4;
        const size_t maxIndexCount = maxQuadCount * 6;

        uint32_t indices[maxIndexCount];
        uint32_t offset = 0;
        for (size_t i = 0; i < maxIndexCount; i += 6) {
            indices[i + 0] = 0 + offset;
            indices[i + 1] = 1 + offset;
            indices[i + 2] = 2 + offset;

            indices[i + 3] = 2 + offset;
            indices[i + 4] = 3 + offset;
            indices[i + 5] = 0 + offset;

            offset += 4;
        }


        shader = std::make_unique<Shader>("res/shaders/BatchRenderingTextures.shader");
        vertexArray = std::make_unique<VertexArray>();
        vertexBuffer = std::make_unique<VertexBuffer>(nullptr, maxVertexCount * sizeof(Vertex), GL_DYNAMIC_DRAW);

        VertexBufferLayout layout;
        layout.push<float>(2);
        layout.push<float>(2);
        layout.push<float>(1);
        vertexArray->addBuffer(*vertexBuffer, layout);

        indexBuffer = std::make_unique<IndexBuffer>(indices, maxIndexCount);

        shader->bind();

        textureA = std::make_unique<Texture>("res/textures/spongebob.png");
        textureB = std::make_unique<Texture>("res/textures/texture01.png");
	}

    TestBatchRenderingDynamic::~TestBatchRenderingDynamic() {}

	void TestBatchRenderingDynamic::onUpdate(float deltaTime) {}

	void TestBatchRenderingDynamic::onRender() {
        GLCall(glClearColor(0.0f, 0.0f, 0.0f, 1.0f));
        GLCall(glClear(GL_COLOR_BUFFER_BIT));

        Renderer renderer;

        vertexBuffer->bind();

        uint32_t indexCount = 0;
        std::array<Vertex, 1000> vertices;
        {
            Vertex* buffer = vertices.data();

            for (int y = 0; y < 600.0f; y += 50.0f) {
                for (int x = 0; x < 600.0f; x += 50.0f) {
                    buffer = createQuad(buffer, x, y, 50.0f, (x / 50 + y / 50) % 2);
                    indexCount += 6;
                }
            }

            buffer = createQuad(buffer, quadPosition[0], quadPosition[1], 100.0f, 0.0f);
            indexCount += 6;
        }

        GLCall(glBufferSubData(GL_ARRAY_BUFFER, 0,  vertices.size() * sizeof(Vertex), vertices.data()));

        textureA->bind(0);
        textureB->bind(1);

        glm::vec3 translation(0, 0, 0);
        glm::mat4 model = glm::translate(glm::mat4(1.0f), translation);
        glm::mat4 mvp = proj * view * model;
        shader->bind();
        shader->setUniformMat4f("u_MVP", mvp);
        int uTextures[2] = { 0, 1 };
        shader->setUniform1iv("u_Textures", uTextures);

        renderer.draw(*vertexArray, *indexBuffer, *shader, indexCount);
	}

	void TestBatchRenderingDynamic::onImGuiRender() {
        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
        ImGui::DragFloat2("Quad Position", quadPosition, 0.1f);
	}

} // namespace test