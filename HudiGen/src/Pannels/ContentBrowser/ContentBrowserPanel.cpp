#include "ContentBrowserPanel.h"
#include <Windows.h>

#include <Hudi.h>
#include <imgui.h>

namespace Hudi {

	extern const std::filesystem::path g_AssetsPath = "assets";
	static const std::string s_ContentBrowerIconPath = "resources\\images\\icons\\content_browser\\light\\";

	ContentBrowserPanel::ContentBrowserPanel()
		: m_CurrentPath(g_AssetsPath)
	{
		m_Icons["folder"] = Texture2D::Create(s_ContentBrowerIconPath + "folder.png");
		m_Icons["text"] = Texture2D::Create(s_ContentBrowerIconPath + "txt.png");
		m_Icons["png"] = Texture2D::Create(s_ContentBrowerIconPath + "png.png");
	}

	void ContentBrowserPanel::OnImGuiRender()
	{
		ImGui::Begin("Browser");

		if (m_CurrentPath != g_AssetsPath)
		{
			if (ImGui::Button("<-"))
				m_CurrentPath = m_CurrentPath.parent_path();
		}

		float padding = 16.0f;
		float iconWidth = 64;
		float viewportWidth = ImGui::GetContentRegionAvail().x;

		int n_Columns = (int)viewportWidth / (padding + iconWidth);
		if (n_Columns < 1)
			n_Columns = 1;

		ImGui::Columns(n_Columns, nullptr, false);

		bool isDoubleClick = ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left);
		ImGui::PushStyleColor(ImGuiCol_Button, { 0, 0, 0, 0 });
		int id = 0;
		for (const auto& item : std::filesystem::directory_iterator(m_CurrentPath))
		{
			ImGui::PushID(id++);
			std::filesystem::path itemPath = item.path();

			Ref<Texture2D> icon = nullptr;
			if (item.is_directory())
				icon = m_Icons.at("folder");
			else
			{
				if (itemPath.filename().extension() == ".hud")
					icon = m_Icons.at("text");
				else if (itemPath.filename().extension() == ".png")
					icon = m_Icons.at("png");
			}

			if (icon)
				ImGui::ImageButton((ImTextureID)icon->GetRendererID(), { iconWidth, iconWidth }, { 0, 1 }, { 1, 0 });

			if (item.is_directory() && isDoubleClick && ImGui::IsItemHovered())
			{
				m_CurrentPath = item.path();
			}
			else if (itemPath.filename().extension() == ".hud")
			{
				if (ImGui::BeginDragDropSource())
				{
					ImGui::SetDragDropPayload("CONTENT_BROWSER_PANEL", itemPath.string().c_str(), itemPath.string().size() * sizeof(char) + 1);
					ImGui::EndDragDropSource();
				}
			}
			else if (itemPath.filename().extension() == ".png")
			{
				if (ImGui::BeginDragDropSource())
				{
					ImGui::SetDragDropPayload("CONTENT_BROWSER_PANEL", itemPath.string().c_str(), itemPath.string().size() * sizeof(char) + 1);
					ImGui::EndDragDropSource();
				}
			}

			if (icon)
			{
				ImGui::TextWrapped("%s", itemPath.filename().string().c_str());
				ImGui::NextColumn();
			}

			ImGui::PopID();
		}
		ImGui::PopStyleColor();
		ImGui::Columns(1);

		ImGui::End();
	}

}