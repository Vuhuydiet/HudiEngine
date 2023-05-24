#include "hdpch.h"
#include "Renderer.h"

#include "RenderCommand.h"
#include "Renderer2D.h"

namespace Hudi {

	Renderer::SceneData Renderer::m_Data;

	void Renderer::Init()
	{
		RenderCommand::Init();
		Renderer2D::Init();
	}

	void Renderer::Shutdown()
	{
		Renderer2D::Shutdown();
	}

	void Renderer::OnWindowResize(uint32_t width, uint32_t height)
	{
		RenderCommand::SetViewport(0, 0, width, height);
	}

	void Renderer::BeginScene(const glm::mat4& prj, const glm::mat4& view)
	{
		m_Data.projectionViewMatrix = prj * view;
	}

	void Renderer::EndScene()
	{
	}

	void Renderer::Submit(Ref<Shader> shader, Ref<VertexArray> vertexArray, const glm::mat4& tranform)
	{
		shader->Bind();
		shader->SetUniform("u_ProjectionView", m_Data.projectionViewMatrix);
		shader->SetUniform("u_Transform", tranform);

		vertexArray->Bind();
		RenderCommand::DrawIndexed(vertexArray, vertexArray->GetIndexBuffer()->GetCount());
	}

}