#include "ContentBrowserPanel.h"
#include <Windows.h>

#include <Hudi.h>
#include <imgui.h>
#include <imgui_internal.h>

#include <string>

namespace Hudi {

	extern const std::filesystem::path g_AssetsPath = "assets";
	static const std::filesystem::path s_ContentBrowerIconPath = "resources\\images\\icons\\content_browser\\light\\";

	static bool s_IsOverIcon = false;
	static bool s_IsOverText = false;

	ContentBrowserPanel::ContentBrowserPanel()
		: m_CurrentPath(g_AssetsPath)
	{
		LoadIconTexture({ "folder.png", "text.png", "png.png", "cpp.png", "dll.png"});
	}

	void ContentBrowserPanel::OnImGuiRender()
	{
		s_IsOverIcon = false;
		s_IsOverText = false;

		ImGui::Begin("Browser");

		if (m_CurrentPath != g_AssetsPath)
		{
			if (ImGui::Button("<-"))
				m_CurrentPath = m_CurrentPath.parent_path();
		}

		float padding = 16.0f;
		float iconWidth = 64.0f;
		float viewportWidth = ImGui::GetContentRegionAvail().x;

		int n_Columns = (int)(viewportWidth / (padding + iconWidth));
		if (n_Columns < 1)
			n_Columns = 1;

		ImGui::Columns(n_Columns, nullptr, false);

		bool isDoubleClick = ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left);
		ImGui::PushStyleColor(ImGuiCol_Button, { 0, 0, 0, 0 });
		int id = 0;
		for (const auto& item : std::filesystem::directory_iterator(m_CurrentPath))
		{
			std::filesystem::path itemPath = item.path();

			Ref<Texture2D> icon = nullptr;
			if (item.is_directory())
				icon = m_Icons.at("folder");
			else if (itemPath.extension() == ".hud")
				icon = m_Icons.at("text");
			else if (itemPath.extension() == ".png")
				icon = m_Icons.at("png");
			else if (itemPath.extension() == ".dll")
				icon = m_Icons.at("dll");

			if (!icon)
				continue;

			ImGui::PushID(id++);

			ImGui::ImageButton((ImTextureID)(size_t)icon->GetRendererID(), { iconWidth, iconWidth }, { 0, 1 }, { 1, 0 });
			s_IsOverIcon |= ImGui::IsItemHovered();
			bool isIconHovered = ImGui::IsItemHovered();

			if (item.is_directory() && isDoubleClick && s_IsOverIcon)
			{
				m_CurrentPath = item.path();
				ImGui::PopID();
				break;
			}
			else if (itemPath.extension() == ".hud")
			{
				if (ImGui::BeginDragDropSource())
				{
					ImGui::SetDragDropPayload("CONTENT_BROWSER_HUD", itemPath.string().c_str(), itemPath.string().size() * sizeof(char) + 1);
					ImGui::EndDragDropSource();
				}
			}
			else if (itemPath.extension() == ".png")
			{
				if (ImGui::BeginDragDropSource())
				{
					ImGui::SetDragDropPayload("CONTENT_BROWSER_PNG", itemPath.string().c_str(), itemPath.string().size() * sizeof(char) + 1);
					ImGui::EndDragDropSource();
				}
			}
			else if (itemPath.extension() == ".dll")
			{
				if (ImGui::BeginDragDropSource())
				{
					ImGui::SetDragDropPayload("CONTENT_BROWSER_DLL", itemPath.string().c_str(), itemPath.string().size() * sizeof(char) + 1);
					ImGui::EndDragDropSource();
				}
			}

			if (ImGui::BeginPopupContextItem())
			{
				if (ImGui::MenuItem("Delete"))
				{
					remove(itemPath.string().c_str());
				}
				ImGui::EndPopup();
			}

			//ImGui::TextWrapped("%s", itemPath.filename().string().c_str());
			static char buffer[100];
			strcpy(buffer, itemPath.filename().string().c_str());
			if (ImGui::InputTextEx("##filename", nullptr, buffer, 100, { iconWidth + padding / 2.0f, 0 }, ImGuiInputTextFlags_EnterReturnsTrue))
			{
				std::filesystem::path newPath = itemPath.parent_path() / buffer;
				if (rename(itemPath.string().c_str(), newPath.string().c_str()) == 0)
					itemPath = newPath;
			}
			s_IsOverText |= ImGui::IsItemHovered();
			
			ImGui::PopID();
			ImGui::NextColumn();
		}
		ImGui::PopStyleColor();
		ImGui::Columns(1);

		// Right click
		if (!s_IsOverIcon && !s_IsOverText && ImGui::BeginPopupContextWindow())
		{
			if (ImGui::BeginMenu("New"))
			{
				if (ImGui::MenuItem("Script"))
					CreateScript();
				ImGui::EndMenu();
			}
			ImGui::EndPopup();
		}

		// End Content Browser
		ImGui::End();
	}

	std::filesystem::path ContentBrowserPanel::CreateScript()
	{
		static int cnt = 0;
		cnt++;
		std::string filename = "NewFile" + std::to_string(cnt) + ".cpp";
		std::filesystem::path path = m_CurrentPath / filename;
		std::ofstream out(path);
		out.close();

		return filename;
	}

	void ContentBrowserPanel::LoadIconTexture(std::vector<std::filesystem::path> paths)
	{
		for (auto& path : paths)
		{
			std::string name = path.filename().stem().string();
			m_Icons[name] = Texture2D::Create(s_ContentBrowerIconPath / path);
		}
	}
}