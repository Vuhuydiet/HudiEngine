#pragma once

#include <Hudi.h>
#include <imgui.h>

#include "Editor/MenuBar.h"
#include "Editor/HierarchyPanel.h"
#include "Editor/InspectorPanel.h"

namespace Hudi {

	class EditorLayer : public Layer
	{
	public:
		EditorLayer();
		virtual void OnAttach() override;
		virtual void OnDetach() override;

		virtual void OnUpdate(float dt) override;
		virtual void OnImGuiRender() override;

		virtual void OnEvent(Event& e) override;
	private:
		void OnKeyPressedEvent(Event& e);
	private:
		Ref<Framebuffer> m_Framebuffer;

		glm::vec2 m_ViewportSize;
		float frametime = 0.0f;

	private:
		MenuBar m_MenuBar;
		HierarchyPanel m_HierarchyPanel;
		InspectorPanel m_InspectorPanel;
	};
}

