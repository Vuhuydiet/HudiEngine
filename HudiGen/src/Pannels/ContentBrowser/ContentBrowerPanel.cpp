#include "ContentBrowerPanel.h"
#include <Windows.h>

#include <Hudi.h>
#include <imgui.h>

namespace Hudi {

	static std::filesystem::path s_AbsolutePath = "assets";

	ContentBrowerPanel::ContentBrowerPanel()
		: m_CurrentPath(s_AbsolutePath)
	{
	}

	void ContentBrowerPanel::OnImGuiRender()
	{
		ImGui::Begin("Brower");

		if (m_CurrentPath != s_AbsolutePath)
		{
			if (ImGui::Button("<-"))
				m_CurrentPath = m_CurrentPath.parent_path();
		}

		for (const auto& item : std::filesystem::directory_iterator(m_CurrentPath))
		{
			if (item.is_directory())
			{
				if (ImGui::Button(item.path().filename().string().c_str()))
				{
					m_CurrentPath = item.path();
					break;
				}
			}
			else if (item.path().extension() == ".hud")
			{
				if (ImGui::Button(item.path().filename().string().c_str()))
				{
					ShellExecute(NULL, L"open", item.path().wstring().c_str(), NULL, NULL, SW_SHOWDEFAULT);
				}
			}
		}

		ImGui::End();
	}

}