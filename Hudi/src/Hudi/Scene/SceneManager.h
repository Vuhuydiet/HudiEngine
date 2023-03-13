#pragma once
#include "hdpch.h"

#include "Scene.h"

namespace Hudi {

	class SceneManager
	{
	public:
		SceneManager(const SceneManager&) = delete;
		SceneManager& operator= (const SceneManager&) = delete;

		Scene& CreateNewScene(const char* _name);

		Scene& GetActiveScene();
		Scene& GetScene(uint8_t index);
		Scene& GetScene(const char* _name);

		void LoadScene(uint8_t index);
		void LoadScene(const char* _name);

		const GameObject& GetActiveCamera() { return m_Scenes.at(s_ActiveScene)->GetActiveCamera(); }

		static SceneManager& Get() { static SceneManager s_SceneManager; return s_SceneManager; }
	private:
		uint8_t s_ActiveScene = 0;

		std::unordered_map<const char*, uint8_t> m_StringsToIndexes;
		std::unordered_map<uint8_t, const char*> m_SceneNames;
		std::unordered_map<uint8_t, Ref<Scene>> m_Scenes;

	private:
		SceneManager() {}
	};

}