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
		m_Texture = Texture2D::Create("assets/textures/hz.png");
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
		HD_PROFILE_FUNCTION();
		frametime = dt;

		m_CameraController.OnUpdate(dt);

		Quad checkerboard;
		checkerboard.position = { 0.0f, 0.0f, -0.2f };
		checkerboard.size = { 10.0f, 10.0f };
		checkerboard.texture = m_Texture;
		checkerboard.tilingFactor = 10.0f;

		RenderCommand::SetClearColor({ 0.5f, 0.2f, 0.3f, 1.0f });
		RenderCommand::Clear();

		Renderer2D::BeginScene(m_CameraController.GetCamera());
		{
			Renderer2D::DrawQuad({ 0.1f, 0.2f, 0.0f }, { 0.3f, 0.9f }, { 0.0f, 0.0f, 0.0f, 1.0f });
			Renderer2D::DrawQuad({ m_Color.x, m_Color.y, 0.0f }, { 0.7f, 0.4f }, m_Color);

			Renderer2D::DrawQuad(checkerboard);

			for (float x = -5.0f; x <= 5.0f; x += 0.1f)
			{
				for (float y = -5.0f; y <= 5.0f; y += 0.1f)
				{
					Quad quad;
					quad.position = { x, y, 0.2f };
					quad.color = { (x + 5.0f) / 10.0f, (y + 5.0f) / 10.0f, 0.4f, 1.0f };
					quad.size = { 0.09f, 0.09f };
					Renderer2D::DrawQuad(quad);
				}
			}

			Renderer2D::EndScene();
		}

	}

	void Sandbox2D::OnImGuiRender()
	{
		HD_PROFILE_FUNCTION();

		ImGui::Begin("Color settings");
		
		ImGui::ColorEdit4("Color", glm::value_ptr(m_Color));

		ImGui::Text("Frame time: %fms, FPS: %f", frametime * 1000.0f, 1.0f / frametime);

		ImGui::End();
	}

}