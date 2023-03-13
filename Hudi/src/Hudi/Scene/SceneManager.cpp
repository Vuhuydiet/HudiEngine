#include "hdpch.h"
#include "SceneManager.h"

namespace Hudi {

	Scene& SceneManager::CreateNewScene(const char* _name)
	{
		Ref<Scene> newScene = NewRef<Scene>();

		m_StringsToIndexes[_name] = newScene->GetBuildIndex();
		m_SceneNames[newScene->GetBuildIndex()] = _name;
		m_Scenes[newScene->GetBuildIndex()] = newScene;

		if (!s_ActiveScene)
			s_ActiveScene = newScene->GetBuildIndex();
		return *m_Scenes[newScene->GetBuildIndex()];
	}

	Scene& SceneManager::GetActiveScene()
	{
		if (!s_ActiveScene || m_Scenes.find(s_ActiveScene) == m_Scenes.end())
		{
			static Scene scene;
			HD_CORE_ERROR("Current scene not found.");
			return scene;
		}

		return *m_Scenes[s_ActiveScene];
	}

	Scene& SceneManager::GetScene(uint8_t index)
	{
		for (auto& [name, scene] : m_Scenes)
		{
			if (scene->GetBuildIndex() == index)
				return *scene;
		}
		static Scene scene;
		HD_CORE_ERROR("No scene with index {0} found.", index);
		return scene;
	}

	Scene& SceneManager::GetScene(const char* _name)
	{
		static Scene scene;
		if (m_StringsToIndexes.find(_name) == m_StringsToIndexes.end())
		{
			HD_CORE_ERROR("No scene with name {0} found.", _name);
			return scene;
		}

		return *m_Scenes[m_StringsToIndexes[_name]];
	}

	void SceneManager::LoadScene(uint8_t index)
	{
		for (auto& [name, scene] : m_Scenes)
		{
			if (scene->GetBuildIndex() == index)
			{
				LoadScene(name);
				return;
			}
		}
	}

	void SceneManager::LoadScene(const char* _name)
	{
		if (m_StringsToIndexes.find(_name) == m_StringsToIndexes.end())
			return;

		m_Scenes[s_ActiveScene]->EndScene();
		s_ActiveScene = m_StringsToIndexes[_name];
		m_Scenes[s_ActiveScene]->BeginScene();
	}
}