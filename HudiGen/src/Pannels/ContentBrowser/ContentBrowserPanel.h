#pragma once

#include <Hudi.h>

#include <map>
#include <filesystem>

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