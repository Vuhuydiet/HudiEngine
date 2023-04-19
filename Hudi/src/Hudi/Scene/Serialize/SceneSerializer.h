#pragma once

#include "Hudi/Scene/Scene.h"

namespace Hudi {

	class SceneSerializer
	{
	public:
		SceneSerializer(Ref<Scene> scene);

		void Save(const std::string& filepath);
		bool Open(const std::string& filepath);

	private:
		Ref<Scene> m_Scene;
	};

}