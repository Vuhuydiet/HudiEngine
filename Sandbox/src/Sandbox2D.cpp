#include "hdpch.h"
#include "Sandbox2D.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <imgui.h>

namespace Hudi {

	Sandbox2D::Sandbox2D()
		: m_CameraController(800.0f / 600.0f, true), m_Color(1.0f)
	{
	}

	Sandbox2D::~Sandbox2D()
	{
	}

	void Sandbox2D::OnAttach()
	{
	}

	void Sandbox2D::OnDetach()
	{
	}

	void Sandbox2D::OnEvent(Event& e)
	{
		m_CameraController.OnEvent(e);
	}

	void Sandbox2D::OnUpdate(float dt)
	{
		m_CameraController.OnUpdate(dt);

		RenderCommand::SetClearColor({ 0.8f, 0.2f, 0.3f, 1.0f });
		RenderCommand::Clear();

		Renderer2D::BeginScene(m_CameraController.GetCamera());
		{
			Renderer2D::DrawQuad({ 0.1f, 0.2f, 0.0f }, { 0.3f, 0.9f }, m_Color);
			Renderer2D::DrawQuad({ 0.8f, 0.8f, 0.0f }, { 0.7f, 0.4f }, m_Color);

			Renderer2D::EndScene();
		}
	}

	void Sandbox2D::OnImGuiRender()
	{
		ImGui::Begin("Color settings");
		ImGui::ColorEdit4("Color", glm::value_ptr(m_Color));
		ImGui::End();
	}

}