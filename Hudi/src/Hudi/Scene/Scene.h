#pragma once
#include "hdpch.h"

#include "GameObject.h"
#include "CameraComponent.h"

namespace Hudi {

	class Scene
	{
	public:
		Scene();

		void BeginScene();
		void EndScene();

		uint8_t GetBuildIndex() { return m_BuildIndex; }

		// GameObject relevant
		GameObject& CreateGameObject(const std::string& _name);

		bool HasGameObject(const std::string& _name);
		bool HasGameObject(uint32_t id);

		GameObject& GetGameObject(const std::string& _name);
		GameObject& GetGameObject(uint32_t id);
		Ref<GameObject> GetGameObjectByRef(const std::string& _name);
		Ref<GameObject> GetGameObjectByRef(uint32_t id);

		void DestroyGameObject(const std::string& _name);
		void DestroyGameObject(uint32_t id);

		std::string GetGameObjectName(uint32_t id);

		void SetActiveCamera(GameObject& go);
		const GameObject& GetActiveCamera();

		// Looop through all GameObjects
		template <typename F>
		void each(F&& func);
	private:
		uint8_t m_BuildIndex = 0;
		
		/*std::unordered_map<std::string, Ref<GameObject>> m_GameObjectRefs;
		std::unordered_map<Ref<GameObject>, std::string> m_GameObjectNames;*/

		std::unordered_map<uint32_t, Ref<GameObject>> m_GameObjects;
		
		std::map<uint32_t, std::string> m_EntityToName;
		std::unordered_map<std::string, uint32_t> m_NameToEntity;

		GameObject* m_ActiveCamera = nullptr;
	};


	// --------- Impl ----------------------------------------------------------//
	template <typename F>
	inline void Scene::each(F&& func)
	{
		for (auto& [id, name] : m_EntityToName)
		{
			std::forward<F&&>(func)(id);
		}
	}
}