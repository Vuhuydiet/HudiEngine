#pragma once

#include <Hudi.h>
#include <imgui.h>

#include "Editor/MenuBar.h"
#include "Editor/HierarchyPanel.h"
#include "Editor/InspectorPanel.h"

namespace Hudi {
	class EditorLayer : public Hudi::Layer
	{
	public:
		void OnImGuiRender() override;

	private:
		HierarchyPanel m_HierarchyPanel;
		InspectorPanel m_InspectorPanel;
	};
}

