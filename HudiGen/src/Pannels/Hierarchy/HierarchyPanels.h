#pragma once

#include <Hudi.h>

namespace Hudi {

	class HierarchyPanels
	{
	public:
		HierarchyPanels();

		void OnEvent(Event& event);
		void OnUpdate(float dt);
		void OnImGuiRender(bool& hierarchy, bool& inspector, bool& viewport);
	public:
		void SetContext(Ref<Scene> scene);
		Ref<Scene> GetContext();
		GameObject GetSelectedObject();

		void SetViewportState(int state);

	private:
		void OnImGuiRenderHierarchyPanel(bool& open);
		void OnImGuiRenderInspectorPanel(bool& open);
		void OnImGuiRenderViewportPanel(bool& open);
	private:
		void OnViewportEvent(Event& event);
		void OnViewportUpdate(float dt);

	private:
		Ref<Scene> m_Context = nullptr;
		GameObject m_SelectedObject;

		Ref<Framebuffer> m_Framebuffer;
	};

}