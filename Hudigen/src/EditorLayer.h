#pragma once
#include <string>

#include <Hudi.h>
#include <imgui.h>

#include "Pannels/Hierarchy/HierarchyPanels.h"
#include "Pannels/ContentBrowser/ContentBrowerPanel.h"

namespace Hudi {

	class EditorLayer : public Layer
	{
	public:
		EditorLayer();
		virtual void OnAttach() override;
		virtual void OnDetach() override;

		virtual void OnEvent(Event& event) override;
		virtual void OnUpdate(float dt) override;
		virtual void OnImGuiRender() override;

	private:
		void BeginDockspace();
		void EndDockspace();
		void OnImGuiRenderMenuBar();
		void NewScene();
		void OpenScene();
		void SaveScene();
		void SaveSceneAs();
		void CloseScene();

		void OnKeyPressedEvent(Event& e);

	private:
		std::string m_CurrentScenePath;

		bool m_OpenedHierarchy = true;
		bool m_OpenedInspector = true;
		bool m_OpenedViewport = true;
		HierarchyPanels m_HierarchyPanels;

		ContentBrowerPanel m_ContentBrowerPanel;
	};
}

