#pragma once
#include "hdpch.h"

#include "GameObject.h"
#include "RenderSystem/RenderSystem.h"

#include <glm/glm.hpp>

namespace Hudi {

	class Scene
	{
	public:
		Scene(uint8_t index);
		~Scene();
		uint8_t GetBuildIndex() { return m_BuildIndex; }

		void BeginScene();
		void EndScene();

		void OnUpdate(float dt);
		void Flush();
		
		void OnViewportResize(int width, int height);
		glm::vec2 GetViewportSize() const { return { m_Width, m_Height }; }

		// GameObject relevant
		GameObject CreateEmptyObject(const std::string& _name);
		GameObject CreateGameObject(const std::string& _name);

		bool HasGameObject(const std::string& _name);
		bool HasGameObject(uint32_t id);

		GameObject GetGameObject(const std::string& _name);
		GameObject GetGameObject(uint32_t id);

		void DestroyGameObject(const std::string& _name);
		void DestroyGameObject(uint32_t id);

		std::string GetGameObjectName(uint32_t id);
		void RenameGameObject(const std::string& _name, uint32_t id);

		// Loop through all GameObjects
		template <typename Func>
		void Each(Func&& func);
	private:
		uint8_t m_BuildIndex = 0;
		int m_Width = 0;
		int m_Height = 0;
		
	private:
		std::unordered_map<uint32_t, GameObject> m_GameObjects;
		
		std::map<uint32_t, std::string> m_EntityToName;
		std::unordered_map<const char*, uint32_t> m_NameToEntity;

		std::queue<uint32_t> m_DestroyedObjects = {};
	private:
		Ref<RenderSystem> m_RenderSystem;
		Ref<ECS::World> m_World;

	private:
		friend class SceneSerializer;
	};


	// --------- Impl ----------------------------------------------------------//
	template <typename Func>
	inline void Scene::Each(Func&& func)
	{
		for (auto& [id, object] : m_GameObjects)
		{
			std::forward<Func>(func)(object);
		}
	}
}