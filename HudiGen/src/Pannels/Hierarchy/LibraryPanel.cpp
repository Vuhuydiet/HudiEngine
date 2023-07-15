#include "HierarchyPanels.h"

#include <imgui.h>
#include <imgui_internal.h>

namespace Hudi {

	void HierarchyPanels::OnImGuiRenderLibraryPanel(bool& open)
	{
		if (!open)
			return;

		// Begin Library Panel
		ImGui::Begin("Libraries", &open);

		if (!m_Context)
		{
			ImGui::End();
			return;
		}

		Ref<ScriptEngine> engine = m_Context->GetScriptEngine();
		for (const auto& [libname, behaviours] : engine->GetLibraries())
		{
			ImGuiTreeNodeFlags flags = (behaviours.empty() ? ImGuiTreeNodeFlags_Leaf : ImGuiTreeNodeFlags_None) |
				ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_SpanAvailWidth | 
				ImGuiTreeNodeFlags_AllowItemOverlap | ImGuiTreeNodeFlags_FramePadding | ImGuiTreeNodeFlags_DefaultOpen;
			ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2{ 4, 4 });
			bool opened = ImGui::TreeNodeEx((void*)libname.c_str(), flags, "%s", libname.c_str());
			ImGui::PopStyleVar();
			if (!opened)
				continue;
			
			for (const auto& compName : behaviours)
			{
				ImGuiTreeNodeFlags be_flags = ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_SpanAvailWidth;
				if (ImGui::TreeNodeEx((void*)compName.c_str(), be_flags, "%s", compName.c_str()))
				{
					ImGui::TreePop();
				}
				if (ImGui::BeginDragDropSource())
				{
					ImGui::SetDragDropPayload("BEHAVIOUR_LIBRARY_PANEL", compName.c_str(), compName.size() * sizeof(char) + 1);
					ImGui::EndDragDropSource();
				}
			}

			ImGui::TreePop();
		}
		ImGui::Separator();

		float buttonWidth = 100.0f;
		float windowWidth = ImGui::GetWindowContentRegionMax().x;
		ImGui::SetCursorPosX(windowWidth * 0.5f - buttonWidth * 0.5f);
		if (ImGui::Button("Load Library", { buttonWidth, 0.0f }))
		{
			std::string path = FileDialogs::OpenFile("Library (.dll)\0*.dll\0");
			if (!path.empty())
			{
				engine->LoadScriptLibrary(path);
			}
		}
		if (ImGui::BeginDragDropTarget())
		{
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("LIBRARY_CONTENT_BROWSER_PANEL"))
			{
				std::filesystem::path libpath = (const char*)payload->Data;
				engine->LoadScriptLibrary(libpath);
			}
			ImGui::EndDragDropTarget();
		}

		// End Library Panel
		ImGui::End();
	}

}