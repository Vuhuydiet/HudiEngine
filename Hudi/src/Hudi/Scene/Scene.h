#pragma once
#include "hdpch.h"

#include "GameObject.h"
#include "CameraComponent.h"

namespace Hudi {

	class Scene
	{
	public:
		Scene(uint8_t index);

		void BeginScene();
		void EndScene();

		uint8_t GetBuildIndex() { return m_BuildIndex; }

		void OnUpdate(float dt);

		// GameObject relevant
		Ref<GameObject> CreateGameObject(const std::string& _name);

		bool HasGameObject(const std::string& _name);
		bool HasGameObject(uint32_t id);

		Ref<GameObject> GetGameObject(const std::string& _name);
		Ref<GameObject> GetGameObject(uint32_t id);

		void DestroyGameObject(const std::string& _name);
		void DestroyGameObject(uint32_t id);

		std::string GetGameObjectName(uint32_t id);
		void RenameGameObject(const std::string& _name, uint32_t id);

		void SetActiveCamera(Ref<GameObject> cam);
		const Ref<GameObject> GetActiveCamera();

		// Looop through all GameObjects
		template <typename Func>
		void each(Func&& func);
	private:
		uint8_t m_BuildIndex = 0;
		
		/*std::unordered_map<std::string, Ref<GameObject>> m_GameObjectRefs;
		std::unordered_map<Ref<GameObject>, std::string> m_GameObjectNames;*/

		std::unordered_map<uint32_t, Ref<GameObject>> m_GameObjects;
		
		std::map<uint32_t, std::string> m_EntityToName;
		std::unordered_map<std::string, uint32_t> m_NameToEntity;

		Ref<GameObject> m_ActiveCamera = nullptr;

	private:
		Ref<ECS::World> m_World;
	};


	// --------- Impl ----------------------------------------------------------//
	template <typename Func>
	inline void Scene::each(Func&& func)
	{
		for (auto& [id, name] : m_EntityToName)
		{
			std::forward<Func&&>(func)(id);
		}
	}
}