#pragma once

#include "Test.h"

#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "Texture.h"

#include <memory>

namespace test {

	class TestBatchRenderingTextures : public Test
	{
	public:
		TestBatchRenderingTextures();
		~TestBatchRenderingTextures();

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
	};

} // namespace test