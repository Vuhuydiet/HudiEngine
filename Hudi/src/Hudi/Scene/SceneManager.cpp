#include "hdpch.h"
#include "SceneManager.h"

namespace Hudi {

	const char* SceneManager::s_ActiveScene = nullptr;
	std::unordered_map<const char*, Ref<Scene>> SceneManager::s_Scenes;

	void SceneManager::CreateNewScene(const char* _name)
	{
		s_Scenes[_name] = NewRef<Scene>();
		if (!s_ActiveScene)
			s_ActiveScene = _name;
	}

	Scene& SceneManager::GetActiveScene()
	{
		static Scene scene;
		if (!s_ActiveScene || s_Scenes.find(s_ActiveScene) == s_Scenes.end())
		{
			HD_CORE_ERROR("Current scene not found.");
			return scene;
		}

		return *s_Scenes[s_ActiveScene];
	}

	Scene& SceneManager::GetScene(uint8_t index)
	{
		for (auto& [name, scene] : s_Scenes)
		{
			if (scene->buildIndex == index)
				return *scene;
		}
		static Scene scene;
		HD_CORE_ERROR("No scene with index {0} found.", index);
		return scene;
	}

	Scene& SceneManager::GetScene(const char* _name)
	{
		static Scene scene;
		if (s_Scenes.find(_name) == s_Scenes.end())
		{
			HD_CORE_ERROR("No scene with name {0} found.", _name);
			return scene;
		}

		return *s_Scenes[_name];
	}

	void SceneManager::LoadScene(uint8_t index)
	{
		for (auto& [name, scene] : s_Scenes)
		{
			if (scene->buildIndex == index)
			{
				LoadScene(name);
				return;
			}
		}
	}

	void SceneManager::LoadScene(const char* _name)
	{
		if (s_Scenes.find(_name) == s_Scenes.end())
			return;

		s_Scenes[s_ActiveScene]->EndScene();
		s_ActiveScene = _name;
		s_Scenes[s_ActiveScene]->BeginScene();
	}
}