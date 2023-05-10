#include "HierarchyPanels.h"

#include <imgui.h>
#include <Hudi.h>

namespace Hudi {

	static bool s_OnGameObjectClick = false;

	static void DrawGameObjectNode(Ref<Scene> context, GameObject& selectedObject, const std::string& name, GameObject object);
	void HierarchyPanels::OnImGuiRenderHierarchyPanel(bool& open)
	{
		if (!open)
			return;
		
		// Begin hierarchy
		ImGui::Begin("Hierarchy", &open);
		if (m_Context)
		{
			m_Context->Each([&](GameObject object) {
				const std::string& name = m_Context->GetGameObjectName(object);
				DrawGameObjectNode(m_Context, m_SelectedObject, name, object);
			});

			if (ImGui::IsMouseDown(0) && ImGui::IsWindowHovered())
			{
				m_SelectedObject.Reset();
			}

			if (!s_OnGameObjectClick && ImGui::BeginPopupContextWindow())
			{
				if (ImGui::MenuItem("Create new GameObject"))
					m_Context->CreateGameObject("New GameObject");

				ImGui::EndPopup();
			}

			m_Context->Flush();
		}
		// End hierarchy
		ImGui::End();

		s_OnGameObjectClick = false;
	}

	static void DrawGameObjectNode(Ref<Scene> context, GameObject& selectedObject, const std::string& name, GameObject object)
	{
		ImGuiTreeNodeFlags flags =	(selectedObject == object ? ImGuiTreeNodeFlags_Selected : 0) |
									ImGuiTreeNodeFlags_Leaf | //(gameObject.GetChildren().size() != 0? ImGuiTreeNodeFlags_OpenOnArrow : ImGuiTreeNodeFlags_Leaf) |
									ImGuiTreeNodeFlags_SpanAvailWidth;
		bool opened = ImGui::TreeNodeEx((void*)(uint64_t)object.GetEntityID(), flags, name.c_str());

		if (ImGui::IsItemClicked())
		{
			selectedObject = object;
		}

		bool entityDeleted = false;
		if (ImGui::BeginPopupContextItem())
		{
			s_OnGameObjectClick = true;
			if (ImGui::MenuItem("Delete GameObject"))
				entityDeleted = true;

			ImGui::EndPopup();
		}

		if (entityDeleted)
		{
			if (selectedObject == object)
			{
				selectedObject.Reset();
			}
			context->DestroyGameObject(object);
		}

		if (opened)
		{
			/*for (auto child : gameObject->GetChildren())
			{
				std::string& child_name = scene->GetGameObjectName(child->GetEntityID());
				DrawGameObjectNode(child_name, child);
			}*/
			ImGui::TreePop();
		}

	}

}