#include "HierarchyPanels.h"

#include <cmath>
#include <Hudi/Math/Math.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <imgui.h>
#include <ImGuizmo.h>

#include "EditorCamera.h"

namespace Hudi {

	static int s_State = 0;
	static bool s_IsFocus = false;
	static bool s_IsHovered = false;
	static glm::vec2 s_ViewportSize(800, 600);
	glm::vec2 s_ViewportBounds[2];

	static int s_GizmosMod = ImGuizmo::OPERATION::TRANSLATE;
	static bool s_IsUsingGizmos = false;
	static bool s_IsOverGizmos = false;
	static EditorCamera s_EditorCamera;

	static Ref<Framebuffer> s_Framebuffer = nullptr;

	void HierarchyPanels::SetViewportState(int state)
	{
		s_State = state;
	}

	void HierarchyPanels::OnViewportEvent(Event& event)
	{
		if (!s_IsFocus)
			return;

		if (s_State == 0)
		{
			s_EditorCamera.OnEvent(event);

			if (event.type() == EventType::KEY_DOWN)
			{
				switch (event.GetKeyCode())
				{
				case Key::Q:
					if (!ImGuizmo::IsUsing())
					{
						s_GizmosMod++;
						s_GizmosMod = (s_GizmosMod + 1) % 4 - 1;
					}
					break;
				}
			}
		}

	}

	static void OnViewportEditUpdate(float dt, Ref<Scene> context, GameObject& selectedObject);
	static void OnViewportRuntimeUpdate(float dt, Ref<Scene> context);
	void HierarchyPanels::OnViewportUpdate(float dt)
	{
		s_Framebuffer = m_Framebuffer;

		if (FramebufferSpecification spec = m_Framebuffer->GetSpecification();
			s_ViewportSize.x > 0.0f && s_ViewportSize.y > 0.0f && // zero sized framebuffer is invalid
			(spec.width != s_ViewportSize.x || spec.height != s_ViewportSize.y))
		{
			s_Framebuffer->Resize((int)s_ViewportSize.x, (int)s_ViewportSize.y);
			if (m_Context)
				m_Context->OnViewportResize((int)s_ViewportSize.x, (int)s_ViewportSize.y);
			s_EditorCamera.SetViewportSize(s_ViewportSize.x, s_ViewportSize.y);
		}

		switch (s_State)
		{
		case 0:
			OnViewportEditUpdate(dt, m_Context, m_SelectedObject);
			break;
		case 1:
			OnViewportRuntimeUpdate(dt, m_Context);
			break;
		}
	}

	void OnViewportEditUpdate(float dt, Ref<Scene> context, GameObject& selectedObject)
	{
		if (s_IsFocus)
		{
			s_EditorCamera.OnUpdate(dt);
		}

		s_Framebuffer->Bind();

		RenderCommand::SetClearColor(67, 70, 75, 255);
		RenderCommand::Clear();

		// Clear attachment
		s_Framebuffer->ClearAttachment(1, 0);

		if (context)
		{
			Renderer2D::BeginScene(s_EditorCamera.GetViewProjection());

			context->Each([&context](GameObject object) {
				if (!object.HasComponent<Transform>() || !object.HasComponent<SpriteRenderer>())
					return;

				const Transform& transform = object.GetComponent<Transform>();
				const SpriteRenderer& sprite = object.GetComponent<SpriteRenderer>();

				Quad quad;
				quad.transform = transform.Transformation();
				quad.size = sprite.size;
				quad.texture = sprite.texture;
				quad.color = sprite.color;
				Renderer2D::DrawQuad(quad, object.GetEntityID());
				});

			Renderer2D::EndScene();
		}
		//HD_INFO("Over: {0}", s_IsOverGizmos);
		if (s_IsFocus && Input::IsMousePressed(Mouse::BUTTON_LEFT) && !Input::IsKeyDown(Key::L_ALT))
		{
			auto [mx, my] = ImGui::GetMousePos();
			mx -= s_ViewportBounds[0].x;
			my -= s_ViewportBounds[0].y;
			glm::vec2 viewportSize = s_ViewportBounds[1] - s_ViewportBounds[0];
			my = (int)viewportSize.y - my;
			int mouseX = (int)mx, mouseY = (int)my;

			if (!s_IsOverGizmos && mouseX >= 0 && mouseX < (int)viewportSize.x && mouseY >= 0 && mouseY < (int)viewportSize.y)
			{
				int pixelData = s_Framebuffer->ReadPixel(1, mouseX, mouseY);
				if (pixelData != 0)
				{
					selectedObject = context->GetGameObject(pixelData);
				}
				else
				{
					selectedObject.Reset();
				}
			}
		}

		s_Framebuffer->Unbind();
	}

	void OnViewportRuntimeUpdate(float dt, Ref<Scene> context)
	{
		s_Framebuffer->Bind();
		//SceneManager::Get().GetActiveScene()->OnUpdate(dt);

		RenderCommand::SetClearColor(67, 70, 75, 255);
		RenderCommand::Clear();

		if (context)
		{
			GameObject camera = context->GetPrimaryCamera();
			if (camera.Valid())
			{
				const glm::mat4& cameraProjection = camera.GetComponent<Camera>().GetProjection();
				const glm::mat4& cameraTranform = camera.GetComponent<Transform>().Transformation();
				Renderer2D::BeginScene(cameraProjection, cameraTranform);

				context->Each([&](GameObject object) {
					if (!object.HasComponent<Transform>() || !object.HasComponent<SpriteRenderer>())
						return;

					const Transform& transform = object.GetComponent<Transform>();
					const SpriteRenderer& sprite = object.GetComponent<SpriteRenderer>();

					Quad quad;
					quad.transform = transform.Transformation();
					quad.size = sprite.size;
					quad.texture = sprite.texture;
					quad.color = sprite.color;
					Renderer2D::DrawQuad(quad);
				});

				Renderer2D::EndScene();
			}
		}

		s_Framebuffer->Unbind();
	}


	void HierarchyPanels::OnImGuiRenderViewportPanel(bool& open)
	{
		if (!open)
			return;

		ImGuiWindowFlags flags = 0;
		flags |= ImGuiWindowFlags_NoTitleBar;

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0, 0 });
		ImGui::Begin("Viewport", &open, flags);
		ImGui::PopStyleVar();

		s_IsFocus = ImGui::IsWindowFocused();
		s_IsHovered = ImGui::IsWindowHovered();

		auto viewportMinRegion = ImGui::GetWindowContentRegionMin();
		auto viewportMaxRegion = ImGui::GetWindowContentRegionMax();
		auto viewportOffset = ImGui::GetWindowPos();
		s_ViewportBounds[0] = { viewportMinRegion.x + viewportOffset.x, viewportMinRegion.y + viewportOffset.y };
		s_ViewportBounds[1] = { viewportMaxRegion.x + viewportOffset.x, viewportMaxRegion.y + viewportOffset.y };

		// Resize
		ImVec2 size = ImGui::GetContentRegionAvail();
		s_ViewportSize = { size.x, size.y };

		size_t texIndex = s_Framebuffer->GetColorAttachmentID();
		ImGui::Image((void*)texIndex, ImVec2{ size.x, size.y }, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });

		if (s_State != 0)
			return;

		// Gizmos
		s_IsUsingGizmos = false;
		s_IsOverGizmos = false;
		static uint32_t wait = 0;
		if (!m_SelectedObject.Valid())
		{
			wait = 0;
		}
		else
		{
			wait++;
		}
		if (s_GizmosMod != -1 && m_SelectedObject.Valid() && wait > 1) 
		{
			ImGuizmo::SetOrthographic(false);
			ImGuizmo::SetDrawlist();
			ImGuizmo::SetRect(s_ViewportBounds[0].x, s_ViewportBounds[0].y, s_ViewportBounds[1].x - s_ViewportBounds[0].x, s_ViewportBounds[1].y - s_ViewportBounds[0].y);

			const glm::mat4& prj = s_EditorCamera.GetProjectionMatrix();
			const glm::mat4& view = s_EditorCamera.GetViewMatrix();

			// Selected Object
			Transform& tc = m_SelectedObject.GetComponent<Transform>();
			glm::mat4 transform = tc.Transformation();

			glm::mat4 I = glm::mat4(1.0f);
			ImGuizmo::Manipulate(glm::value_ptr(view), glm::value_ptr(prj), (ImGuizmo::OPERATION)s_GizmosMod, ImGuizmo::LOCAL, glm::value_ptr(transform));

			s_IsUsingGizmos = ImGuizmo::IsUsing();
			if (s_IsUsingGizmos)
			{
				glm::vec3 position, rotation, scale;
				Math::DecomposeTransform(transform, position, rotation, scale);
				tc.position = position;
				//tc.rotation += { glm::degrees(rotation.x) - fmod(tc.rotation.x, 360.0f), glm::degrees(rotation.y) - fmod(tc.rotation.y, 360.0f), glm::degrees(rotation.z) - fmod(tc.rotation.z, 360.0f) };
				//rotation = { glm::degrees(rotation.x) + (rotation.x > 0.0f ? 0.0f : 180.0f), glm::degrees(rotation.y) + (rotation.y > 0.0f ? 0.0f : 180.0f), glm::degrees(rotation.z) + (rotation.z > 0.0f ? 0.0f : 180.0f) };
				tc.rotation = glm::degrees(rotation);
				tc.scale = scale;
			}
			s_IsOverGizmos = ImGuizmo::IsOver();

		}

		ImGui::End();
	}


}