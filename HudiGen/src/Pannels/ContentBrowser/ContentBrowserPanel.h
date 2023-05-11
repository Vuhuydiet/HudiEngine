#pragma once

#include <filesystem>

#include <Hudi.h>

namespace Hudi {

	class ContentBrowserPanel
	{
	public:
		ContentBrowserPanel();

		void OnImGuiRender();

	private:
		std::filesystem::path m_CurrentPath;

	private:
		Ref<Texture2D> m_FolderIcon;
		Ref<Texture2D> m_TextFileIcon;
	};

}