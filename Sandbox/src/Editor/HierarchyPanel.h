#pragma once

#include <Hudi.h>

#include "MenuBar.h"

namespace Hudi {

	class HierarchyPanel
	{
	public:
		void OnImGuiRender();

	private:
		void DrawGameObjectNode(std::string& name, Ref<GameObject> gameObject);
	private:
		std::vector<uint32_t> m_OnDelete;

		bool m_OnGameObjectClick = false;
	};

}