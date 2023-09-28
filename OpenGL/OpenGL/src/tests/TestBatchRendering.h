#pragma once

#include "Test.h"

#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "Texture.h"

#include <memory>

namespace test {

	class TestBatchRendering : public Test
	{
	public:
		TestBatchRendering();
		~TestBatchRendering();

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

		glm::mat4 proj;
		glm::mat4 view;
	};

} // namespace test