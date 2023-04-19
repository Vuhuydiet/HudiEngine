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

		FramebufferSpecification specs;
		specs.width = 800;
		specs.height = 600;
		m_Framebuffer = Framebuffer::Create(specs);
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

		m_Framebuffer->Bind();
		RenderCommand::SetClearColor({ 0.5f, 0.2f, 0.3f, 1.0f });
		RenderCommand::Clear();

		Renderer2D::BeginScene(m_CameraController.GetCamera());
		{
			Quad checkerboard;
			checkerboard.transform = glm::translate(glm::mat4(1.0f), { -0.1f, 0.0f, -0.2f });
			checkerboard.size = { 10.0f, 10.0f };
			checkerboard.texture = m_Texture;
			checkerboard.tilingFactor = 10.0f;
			Renderer2D::DrawQuad(checkerboard);

			for (float x = -5.0f; x <= 5.0f; x += 0.5f)
			{
				for (float y = -5.0f; y <= 5.0f; y += 0.5f)
				{
					Quad quad;
					quad.transform = glm::translate(glm::mat4(1.0f), glm::vec3{ x, y, 0.5f});
					quad.color = { (x + 5.0f) / 10.0f, (y + 5.0f) / 10.0f, 0.4f, 1.0f };
					quad.size = { 0.45f, 0.45f };
					Renderer2D::DrawQuad(quad);
				}
			}

			Renderer2D::DrawQuad({ 0.1f, 0.2f, 0.0f }, { 0.3f, 0.9f }, { 0.0f, 0.0f, 0.0f, 1.0f });
			Renderer2D::DrawQuad({ m_Color.x, m_Color.y, 0.1f }, { 0.7f, 0.4f }, m_Color);

			Renderer2D::EndScene();
		}
		m_Framebuffer->Unbind();

	}

	void Sandbox2D::OnImGuiRender()
	{
		const ImGuiViewport* viewport = ImGui::GetMainViewport();
		ImGui::SetNextWindowPos(viewport->WorkPos);
		ImGui::SetNextWindowSize(viewport->WorkSize);
		ImGui::SetNextWindowViewport(viewport->ID);

		ImGuiDockNodeFlags_ dockspace_flags = ImGuiDockNodeFlags_None; // ImGuiDockNodeFlags_PassthruCentralNode;
		ImGuiWindowFlags host_window_flags = ImGuiWindowFlags_None;

		host_window_flags |= ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoDocking;
		host_window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

		if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
			host_window_flags |= ImGuiWindowFlags_NoBackground;

		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));

		// Begin docking window
		ImGui::Begin("Dockspace", NULL, host_window_flags);
		ImGui::PopStyleVar(3);

		ImGuiID dockspace_id = ImGui::GetID("DockSpace");
		ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
		// -------------------------------------------- //

		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("File"))
			{
				ImGui::Separator();
				if (ImGui::MenuItem("Close", NULL, false, true))
				{
					Application::Get().Close();
				}

				ImGui::EndMenu();
			}

			ImGui::EndMenuBar();
		}

		// -------------------------------------------- //
		ImGui::Begin("Game");
		uint32_t texID = m_Framebuffer->GetColorAttachmentID();
		ImGui::Image((void*)(size_t)texID, ImVec2{ 800, 600 });
		ImGui::End();

		ImGui::Begin("Color Edit");
		ImGui::ColorEdit4("Color", glm::value_ptr(m_Color));
		ImGui::End();

		// End docking window
		ImGui::End();
	}

}