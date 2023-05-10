#pragma once

#include <filesystem>

namespace Hudi {

	class ContentBrowerPanel
	{
	public:
		ContentBrowerPanel();

		void OnImGuiRender();

	private:
		std::filesystem::path m_CurrentPath;
	};

}