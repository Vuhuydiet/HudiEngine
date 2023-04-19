#include "EditorLayer.h"

#include <imgui.h>

namespace Hudi {

	EditorLayer::EditorLayer()
		: m_ViewportSize(800, 600)
	{
		Ref<Scene> scene = NewRef<Scene>(0);
		m_HierarchyPanel.SetContext(scene);

		GameObject mainCamera = scene->CreateGameObject("Main Camera");
		Camera& cameraComponent = mainCamera.AddComponent<Camera>();
		mainCamera.RemoveComponent<SpriteRenderer>();

		GameObject vudeptrai = scene->CreateGameObject("vu dep trai");
		//auto trai = scene->CreateGameObject("trai");

		//vudeptrai->AddChild(trai);
		//trai->SetParent(vudeptrai);
	}

	void EditorLayer::OnAttach()
	{
		FramebufferSpecification specs;
		specs.width = 1280;
		specs.height = 720;
		m_Framebuffer = Framebuffer::Create(specs);
	}

	void EditorLayer::OnDetach()
	{
	}

	void EditorLayer::OnUpdate(float dt)
	{
		frametime = dt;

		m_Framebuffer->Bind();
		//SceneManager::Get().GetActiveScene()->OnUpdate(dt);

		RenderCommand::SetClearColor(169, 169, 169, 255);
		RenderCommand::Clear();

		Ref<Scene> scene = m_HierarchyPanel.GetContext();

		GameObject camera;
		scene->Each([&](GameObject object) {
			if (object.HasComponent<Camera>())
				camera = object;
		});

		if (camera.Valid())
		{
			const glm::mat4& cameraProjection = camera.GetComponent<Camera>().GetProjection();
			const glm::mat4& cameraTranform = camera.GetComponent<Transform>().TransformationMatrix();
			Renderer2D::BeginScene(cameraProjection, cameraTranform);

			scene->Each([&](GameObject object) {
				if (!object.HasComponent<Transform>() || !object.HasComponent<SpriteRenderer>())
					return;
				
				const Transform& transform = object.GetComponent<Transform>();
				const SpriteRenderer& sprite = object.GetComponent<SpriteRenderer>();

				Quad quad;
				quad.transform = transform.TransformationMatrix();
				quad.size = sprite.size;
				quad.texture = sprite.texture;
				quad.color = sprite.color;
				Renderer2D::DrawQuad(quad);
			});

			Renderer2D::EndScene();
		}

		m_Framebuffer->Unbind();
	}

	void EditorLayer::OnImGuiRender()
	{
		Ref<Scene> scene = m_MenuBar.OnImGuiRender(m_HierarchyPanel.GetContext());
		m_HierarchyPanel.SetContext(scene);
		m_HierarchyPanel.OnImGuiRender(m_MenuBar.opt_Hierarchy);
		m_InspectorPanel.OnImGuiRender(m_MenuBar.opt_Inspector, m_HierarchyPanel.GetSelectedObject());

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0, 0 });
		ImGui::Begin("Viewport");
		ImGui::PopStyleVar();

		ImVec2 size = ImGui::GetContentRegionAvail();
		if (m_ViewportSize != *(glm::vec2*)&size && size.x > 0 && size.y > 0)
		{
			m_ViewportSize = { size.x, size.y };
			m_Framebuffer->Resize(size.x, size.y);
			//SceneManager::Get().GetActiveScene()->OnViewportResize(size.x, size.y);
			m_HierarchyPanel.GetContext()->OnViewportResize(size.x, size.y);
		}
		size_t texIndex = m_Framebuffer->GetColorAttachmentID();
		ImGui::Image((void*)texIndex, ImVec2{ size.x, size.y }, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });

		m_HierarchyPanel.GetContext()->Flush();

		ImGui::End();
	}

	void EditorLayer::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch(KEY_DOWN, HD_BIND_EVENT_FN(EditorLayer::OnKeyPressedEvent));
	}

	void EditorLayer::OnKeyPressedEvent(Event& e)
	{
		bool ctrl = Input::IsKeyDown(Key::L_CTRL) || Input::IsKeyDown(Key::R_CTRL);
		bool shift = Input::IsKeyDown(Key::L_SHIFT) || Input::IsKeyDown(Key::R_SHIFT);
		switch (((SDL_Event)e).key.keysym.sym)
		{
		case Key::N:
			if (ctrl)
				m_HierarchyPanel.SetContext(m_MenuBar.NewScene());
			break;
		case Key::O:
			if (ctrl)
				m_HierarchyPanel.SetContext(m_MenuBar.OpenScene());
			break;
		case Key::S:
			if (ctrl && shift)
				m_MenuBar.SaveSceneAs(m_HierarchyPanel.GetContext());
			break;
		}

		e.handled = true;
	}
}

