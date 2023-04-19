#pragma once
#include "hdpch.h"

#include "Scene.h"

namespace Hudi {

	class SceneManager
	{
	public:
		Ref<Scene> CreateNewScene(const std::string& _name);

		Ref<Scene> GetActiveScene();
		Ref<Scene> GetScene(uint8_t index);
		Ref<Scene> GetScene(const std::string& _name);
		void SetActiveScene(uint8_t index) { m_ActiveScene = index; }

		void DeleteScene(uint8_t index);

		void LoadScene(uint8_t index);
		void LoadScene(const std::string& _name);

		static SceneManager& Get() { static SceneManager s_SceneManager; return s_SceneManager; }
		SceneManager(const SceneManager&) = delete;
		SceneManager& operator= (const SceneManager&) = delete;

	private:
		uint8_t m_ActiveScene = 0;

		std::unordered_map<std::string, uint8_t> m_StringToIndex;
		std::unordered_map<uint8_t, std::string> m_SceneNames;

		std::unordered_map<uint8_t, Ref<Scene>> m_Scenes;
		uint8_t m_BuildIndexCount = 1;

	private:
		SceneManager() {}
	};

}