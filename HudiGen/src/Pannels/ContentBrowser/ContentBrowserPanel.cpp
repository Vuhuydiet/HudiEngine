#include "ContentBrowserPanel.h"
#include <Windows.h>

#include <Hudi.h>
#include <imgui.h>

namespace Hudi {

	extern const std::filesystem::path g_AssetsPath = "assets";
	static const std::string s_ContentBrowerIconPath = "resources\\images\\icons\\content_brower\\light\\";

	ContentBrowserPanel::ContentBrowserPanel()
		: m_CurrentPath(g_AssetsPath)
	{
		m_FolderIcon = Texture2D::Create(s_ContentBrowerIconPath + "folder.png");
		m_TextFileIcon = Texture2D::Create(s_ContentBrowerIconPath + "txt.png");
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

			Ref<Texture2D> iconTexture = nullptr;
			if (item.is_directory())
				iconTexture = m_FolderIcon;
			else if (itemPath.extension() == ".hud")
				iconTexture = m_TextFileIcon;

			if (iconTexture)
				ImGui::ImageButton((ImTextureID)iconTexture->GetRendererID(), { iconWidth, iconWidth }, { 0, 1 }, { 1, 0 });

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

			if (iconTexture)
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