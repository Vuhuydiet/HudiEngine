#include "hdpch.h"
#include "SceneManager.h"

namespace Hudi {

	Ref<Scene> SceneManager::CreateNewScene(const char* _name)
	{
		Ref<Scene> newScene = NewRef<Scene>(m_BuildIndexCount++);

		m_StringsToIndexes[_name] = newScene->GetBuildIndex();
		m_SceneNames[newScene->GetBuildIndex()] = _name;
		m_Scenes[newScene->GetBuildIndex()] = newScene;

		if (!s_ActiveScene)
			s_ActiveScene = newScene->GetBuildIndex();
		return m_Scenes[newScene->GetBuildIndex()];
	}

	Ref<Scene> SceneManager::GetActiveScene()
	{
		if (!s_ActiveScene || m_Scenes.find(s_ActiveScene) == m_Scenes.end())
		{
			HD_CORE_ERROR("Current scene not found. s_ActiveScene: {0}", s_ActiveScene);
			return nullptr;
		}

		return m_Scenes[s_ActiveScene];
	}

	Ref<Scene> SceneManager::GetScene(uint8_t index)
	{
		for (auto& [name, scene] : m_Scenes)
		{
			if (scene->GetBuildIndex() == index)
				return scene;
		}
		HD_CORE_ERROR("No scene with index {0} found.", index);
		return nullptr;
	}

	Ref<Scene> SceneManager::GetScene(const char* _name)
	{
		if (m_StringsToIndexes.find(_name) == m_StringsToIndexes.end())
		{
			HD_CORE_ERROR("No scene with name {0} found.", _name);
			return nullptr;
		}

		return m_Scenes[m_StringsToIndexes[_name]];
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