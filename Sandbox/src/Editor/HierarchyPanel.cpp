#include "HierarchyPanel.h"

#include <imgui.h>

#include <Hudi.h>

namespace Hudi {

	void HierarchyPanel::OnImGuiRender()
	{
		//m_OnDelete.clear();

		//bool& hierarchy = MenuBar::Get().opt_Hierarchy;
		//if (hierarchy)
		//{
		//	// Begin hierarchy
		//	ImGui::Begin("Hierarchy", &hierarchy);

		//	auto& scene = SceneManager::Get().GetActiveScene();
		//	scene->Each([&](uint32_t id) {
		//		std::string& name = scene->GetGameObjectName(id);
		//		Ref<GameObject> gameObject = scene->GetGameObject(id);
		//		if (!gameObject->GetParent())
		//		{
		//			DrawGameObjectNode(name, gameObject);
		//		}
		//	});

		//	for (auto id : m_OnDelete)
		//	{
		//		scene->DestroyGameObject(id);
		//	}

		//	if (ImGui::IsMouseDown(0) && ImGui::IsWindowHovered())
		//	{
		//		MenuBar::Get().SetSelectedObject(nullptr);
		//	}

		//	if (!m_OnGameObjectClick && ImGui::BeginPopupContextWindow(0, 1))
		//	{
		//		if (ImGui::MenuItem("Create new GameObject"))
		//			scene->CreateGameObject("New GameObject");

		//		ImGui::EndPopup();
		//	}
		//	// End hierarchy
		//	ImGui::End();

		//	m_OnGameObjectClick = false;
		//}

	}

	void HierarchyPanel::DrawGameObjectNode(std::string& name, Ref<GameObject> gameObject)
	{
		/*bool opened = false;
		ImGuiTreeNodeFlags flags =	(MenuBar::Get().GetSelectedObject() == gameObject ? ImGuiTreeNodeFlags_Selected : 0) |
									(gameObject->GetChildren().size() != 0? ImGuiTreeNodeFlags_OpenOnArrow : ImGuiTreeNodeFlags_Leaf) |
									ImGuiTreeNodeFlags_SpanAvailWidth;
		opened = ImGui::TreeNodeEx((void*)(uint64_t)(ECS::Entity)*gameObject, flags, name.c_str());

		if (ImGui::IsItemClicked())
		{
			MenuBar::Get().SetSelectedObject(gameObject);
		}

		bool entityDeleted = false;
		if (ImGui::BeginPopupContextItem())
		{
			m_OnGameObjectClick = true;
			if (ImGui::MenuItem("Delete GameObject"))
				entityDeleted = true;

			ImGui::EndPopup();
		}

		auto& scene = SceneManager::Get().GetActiveScene();

		if (entityDeleted)
		{
			if (MenuBar::Get().GetSelectedObject() == gameObject)
			{
				MenuBar::Get().SetSelectedObject(nullptr);
			}
			m_OnDelete.push_back(gameObject->GetEntityID());
		}

		if (opened)
		{
			if (gameObject->GetChildren().size())
			{
				for (auto& child : gameObject->GetChildren())
				{
					std::string& child_name = scene->GetGameObjectName(child->GetEntityID());
					DrawGameObjectNode(child_name, child);
				}
			}
			ImGui::TreePop();
		}*/

	}

}