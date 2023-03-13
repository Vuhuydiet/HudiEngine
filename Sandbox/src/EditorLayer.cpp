#include "EditorLayer.h"

#include <imgui.h>

namespace Hudi {

	void EditorLayer::OnImGuiRender()
	{
		MenuBar::Get().OnImGuiRender();

		m_HierarchyPanel.OnImGuiRender();
		m_InspectorPanel.OnImGuiRender();
	}
}

