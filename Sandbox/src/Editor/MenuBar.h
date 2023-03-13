#pragma once

#include <Hudi.h>

namespace Hudi {

	class MenuBar
	{
	public:
		MenuBar(const MenuBar&) = delete;
		MenuBar& operator= (const MenuBar&) = delete;

		void OnImGuiRender();


		static MenuBar& Get() { static MenuBar s_MenuBar; return s_MenuBar; }
	
		void SetSelectedObject(Ref<GameObject> obj) { m_SelectedObject = obj; }
		Ref<GameObject> GetSelectedObject() { return m_SelectedObject; }
	public:
		bool opt_Hierarchy = true;
		bool opt_Inspector = true;

	private:
		Ref<GameObject> m_SelectedObject = nullptr;
	private:
		MenuBar() {}
	};

}