#pragma once
#include "hdpch.h"

#include "Scene.h"

namespace Hudi {

	class SceneManager
	{
	public:

		static void CreateNewScene(const char* _name);

		static Scene& GetActiveScene();
		static Scene& GetScene(uint8_t index);
		static Scene& GetScene(const char* _name);

		static Camera* GetActiveCamera() { return s_Scenes.at(s_ActiveScene)->GetActiveCamera(); }

		static void LoadScene(uint8_t index);
		static void LoadScene(const char* _name);

	private:
		static const char* s_ActiveScene;
		static std::unordered_map<const char*, Ref<Scene>> s_Scenes;
	};

}