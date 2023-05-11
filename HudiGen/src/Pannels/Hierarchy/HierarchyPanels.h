#pragma once

#include <string>

#include <Hudi.h>

namespace Hudi {

	struct PanelCommand
	{
		enum Type { None, OpenScene };

		Type type;
		void* data;
	};

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

		bool PollCommand(PanelCommand& command);

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

		std::queue<PanelCommand> m_Commands;

		Ref<Framebuffer> m_Framebuffer;
	};

}