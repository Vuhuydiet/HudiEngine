#include "hdpch.h"
#include "Renderer2D.h"

#include <glm/gtc/matrix_transform.hpp>

namespace Hudi{

	struct RendererData
	{
		Ref<VertexArray> vertexArray;
		Ref<Shader> shader;
	};

	static RendererData* s_Data = nullptr;

	void Renderer2D::Init()
	{
		s_Data = new RendererData();

		s_Data->vertexArray = VertexArray::Create();

		float vertices[4 * 5] = {
			-0.5f, -0.5f, 0.0f,
			 0.5f, -0.5f, 0.0f,
			 0.5f,  0.5f, 0.0f,
			-0.5f,  0.5f, 0.0f
		};
		Ref<VertexBuffer> vertexBuffer = VertexBuffer::Create(vertices, sizeof(vertices));
		vertexBuffer->SetLayout({
			{ShaderDataType::Float3, "a_Position"}
		});
		s_Data->vertexArray->AddVertexBuffer(vertexBuffer);

		uint32_t indices[6] = { 0, 1, 2, 2, 3, 0 };
		Ref<IndexBuffer> indexBuffer = IndexBuffer::Create(indices, GET_COUNT(indices));
		s_Data->vertexArray->SetIndexBuffer(indexBuffer);

		s_Data->shader = Shader::Create("assets/shaders/Rect.glsl");
	}

	void Renderer2D::Shutdown()
	{
		HD_CORE_ASSERT(!s_Data, "The data has not been init yet!");

		delete s_Data;
		s_Data = nullptr;
	}

	void Renderer2D::BeginScene(OrthographicCamera& camera)
	{
		s_Data->shader->Bind();
		s_Data->shader->SetUniform("u_ProjectionViewMatrix", camera.GetProjectionViewMatrix());
	}

	void Renderer2D::EndScene()
	{
	}

	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color)
	{
		DrawQuad(glm::vec3(position.x, position.y, 0.0f), size, color);
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color)
	{
		s_Data->shader->Bind();
		s_Data->shader->SetUniform("u_Color", color);

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) * glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });
		s_Data->shader->SetUniform("u_Transform", transform);

		s_Data->vertexArray->Bind();
		RenderCommand::DrawIndexed(s_Data->vertexArray);
	}

}