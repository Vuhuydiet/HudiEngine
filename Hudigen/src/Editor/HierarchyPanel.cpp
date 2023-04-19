#include "HierarchyPanel.h"

#include <imgui.h>

#include <Hudi.h>

namespace Hudi {

	void HierarchyPanel::OnImGuiRender(bool& open)
	{
		if (!open)
			return;
		
		// Begin hierarchy
		ImGui::Begin("Hierarchy", &open);

		m_Context->Each([&](GameObject object) {
			const std::string& name = m_Context->GetGameObjectName(object.GetEntityID());
			DrawGameObjectNode(name, object);
		});

		if (ImGui::IsMouseDown(0) && ImGui::IsWindowHovered())
		{
			m_SelectedObject.Reset();
		}

		if (!m_OnGameObjectClick && ImGui::BeginPopupContextWindow(0, 1))
		{
			if (ImGui::MenuItem("Create new GameObject"))
				m_Context->CreateGameObject("New GameObject");

			ImGui::EndPopup();
		}
		// End hierarchy
		ImGui::End();

		m_OnGameObjectClick = false;

	}

	void HierarchyPanel::SetContext(Ref<Scene> scene)
	{
		if (m_Context == scene)
			return;
		m_Context = scene;
		m_SelectedObject.Reset();
	}

	Ref<Scene> HierarchyPanel::GetContext()
	{
		return m_Context;
	}

	GameObject HierarchyPanel::GetSelectedObject()
	{
		return m_SelectedObject;
	}

	void HierarchyPanel::DrawGameObjectNode(const std::string& name, GameObject gameObject)
	{
		ImGuiTreeNodeFlags flags =	(m_SelectedObject == gameObject ? ImGuiTreeNodeFlags_Selected : 0) |
									ImGuiTreeNodeFlags_Leaf | //(gameObject.GetChildren().size() != 0? ImGuiTreeNodeFlags_OpenOnArrow : ImGuiTreeNodeFlags_Leaf) |
									ImGuiTreeNodeFlags_SpanAvailWidth;
		bool opened = ImGui::TreeNodeEx((void*)(uint64_t)gameObject.GetEntityID(), flags, name.c_str());

		if (ImGui::IsItemClicked())
		{
			m_SelectedObject = gameObject;
		}

		bool entityDeleted = false;
		if (ImGui::BeginPopupContextItem())
		{
			m_OnGameObjectClick = true;
			if (ImGui::MenuItem("Delete GameObject"))
				entityDeleted = true;

			ImGui::EndPopup();
		}

		if (entityDeleted)
		{
			if (m_SelectedObject == gameObject)
			{
				m_SelectedObject.Reset();
			}
			m_Context->DestroyGameObject(gameObject.GetEntityID());
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