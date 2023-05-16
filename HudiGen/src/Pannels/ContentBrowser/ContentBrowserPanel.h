#pragma once

#include <map>
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
		std::map<std::string, Ref<Texture2D>> m_Icons;
	};

}