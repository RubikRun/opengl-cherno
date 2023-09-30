#pragma once

#include "Test.h"

#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "Texture.h"

#include <memory>

namespace test {

	class TestBatchRenderingDynamic : public Test
	{
	public:
		TestBatchRenderingDynamic();
		~TestBatchRenderingDynamic();

		void onUpdate(float deltaTime) override;
		void onRender() override;
		void onImGuiRender() override;
	private:
		float windowWidth;
		float windowHeight;

		std::unique_ptr<VertexBuffer> vertexBuffer;
		std::unique_ptr<VertexArray> vertexArray;
		std::unique_ptr<IndexBuffer> indexBuffer;
		std::unique_ptr<Shader> shader;
		std::unique_ptr<Texture> textureA;
		std::unique_ptr<Texture> textureB;

		glm::mat4 proj;
		glm::mat4 view;

		float quadPosition[2] = { 100.0f, 100.0f };
	};

} // namespace test