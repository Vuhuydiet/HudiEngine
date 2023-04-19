#include "hdpch.h"
#include "SceneManager.h"

namespace Hudi {

	Ref<Scene> SceneManager::CreateNewScene(const std::string& _name)
	{
		std::string name = _name;

		static uint8_t ind = 1;
		if (m_StringToIndex.find(name) != m_StringToIndex.end())
			name += " (" + std::to_string(ind++) + ")";

		Ref<Scene> newScene = NewRef<Scene>(m_BuildIndexCount++);

		m_StringToIndex[name] = newScene->GetBuildIndex();
		m_SceneNames[newScene->GetBuildIndex()] = name;
		m_Scenes[newScene->GetBuildIndex()] = newScene;

		if (!m_ActiveScene)
			m_ActiveScene = newScene->GetBuildIndex();
		return newScene;
	}

	Ref<Scene> SceneManager::GetActiveScene()
	{
		if (!m_ActiveScene || m_Scenes.find(m_ActiveScene) == m_Scenes.end())
		{
			HD_CORE_ERROR("Current scene not found. s_ActiveScene: {0}", m_ActiveScene);
			return nullptr;
		}

		return m_Scenes[m_ActiveScene];
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

	Ref<Scene> SceneManager::GetScene(const std::string& _name)
	{
		if (m_StringToIndex.find(_name) == m_StringToIndex.end())
		{
			HD_CORE_ERROR("No scene with name {0} found.", _name);
			return nullptr;
		}

		return m_Scenes[m_StringToIndex[_name]];
	}

	void SceneManager::DeleteScene(uint8_t index)
	{
		if (m_Scenes.find(index) == m_Scenes.end())
		{
			HD_CORE_WARN("Deleting a scene that does not exist! (index {0})", index);
			return;
		}
		
		const std::string& name = m_SceneNames[index];
		m_StringToIndex.erase(name);
		m_SceneNames.erase(index);
		m_Scenes.erase(index);
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

	void SceneManager::LoadScene(const std::string& _name)
	{
		if (m_StringToIndex.find(_name) == m_StringToIndex.end())
			return;

		m_Scenes[m_ActiveScene]->EndScene();
		m_ActiveScene = m_StringToIndex[_name];
		m_Scenes[m_ActiveScene]->BeginScene();
	}
}