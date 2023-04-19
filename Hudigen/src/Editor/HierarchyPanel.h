#pragma once

#include <Hudi.h>

#include "MenuBar.h"

namespace Hudi {

	class HierarchyPanel
	{
	public:
		void OnImGuiRender(bool& open);

		void SetContext(Ref<Scene> scene);
		Ref<Scene> GetContext();
		
		GameObject GetSelectedObject();

	private:

		void DrawGameObjectNode(const std::string& name, GameObject gameObject);
	private:
		Ref<Scene> m_Context = nullptr;
		GameObject m_SelectedObject;

		bool m_OnGameObjectClick = false;
	};

}