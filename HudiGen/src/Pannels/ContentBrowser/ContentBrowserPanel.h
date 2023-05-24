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
		std::filesystem::path CreateScript();

		void LoadIconTexture(std::vector<std::filesystem::path> paths);

	private:
		std::filesystem::path m_CurrentPath;

	private:
		// resources
		std::map<std::string, Ref<Texture2D>> m_Icons;
	};

}