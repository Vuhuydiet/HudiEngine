#pragma once

#include <string>

#include <Hudi.h>

namespace Hudi {

	struct PanelCommand
	{
		enum Type { None = 0, OpenScene };

		PanelCommand() = default;
		PanelCommand(Type _type, const void* _data, size_t _dataSize);
		PanelCommand(const PanelCommand& other);
		~PanelCommand();
		PanelCommand& operator= (const PanelCommand& other);

		Type type = Type::None;
		void* data = nullptr;
		size_t dataSize = 0;
	};

	enum ViewportState { None = 0, Edit, Runtime };

	class HierarchyPanels
	{
	public:
		HierarchyPanels();

		void SetContext(Ref<Scene> scene);
		Ref<Scene> GetContext();
		GameObject GetSelectedObject();

	public:
		void OnEvent(Event& event);
		void OnUpdate(float dt);
		void OnImGuiRender(bool& hierarchy, bool& inspector, bool& viewport, bool& lib);
	
	public:
		bool PollCommand(PanelCommand& command);

		bool IsHierarchyFocused() const;
		bool IsInspectorFocused() const;
		bool IsViewportFocused() const;
		bool IsHierarchyHovered() const;
		bool IsInspectorHovered() const;
		bool IsViewportHovered() const;

		ViewportState GetViewportState() const;
		void SetViewportPlay();
		void SetViewportEdit();

	private:
		void OnImGuiRenderHierarchyPanel(bool& open);
		void OnImGuiRenderInspectorPanel(bool& open);
		void OnImGuiRenderViewportPanel(bool& open);
		void OnImGuiRenderLibraryPanel(bool& open);
	private:
		void OnViewportEvent(Event& event);
		void OnViewportUpdate(float dt);

	private:
		Ref<Scene> m_Context = nullptr;
		GameObject m_SelectedObject;

		std::queue<PanelCommand> m_Commands;

		Ref<Framebuffer> m_Framebuffer;
		std::map<std::string, Ref<Texture2D>> m_Icons;
	};

}