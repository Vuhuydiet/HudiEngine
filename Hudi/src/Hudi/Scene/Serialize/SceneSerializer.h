#pragma once

#include <filesystem>

#include "Hudi/Scene/Scene.h"

namespace Hudi {

	class SceneSerializer
	{
	public:
		SceneSerializer(Ref<Scene> scene);

		void Save(const std::filesystem::path& filepath);
		bool Open(const std::filesystem::path& filepath);

	private:
		Ref<Scene> m_Scene;
	};

}