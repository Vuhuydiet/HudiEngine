#pragma once
#include "Hudi/Core/Core.h"

#include "GameObject.h"
#include "RenderSystem/RenderSystem.h"
#include "Physics/Physics2D/Physics2DSystem.h"

#include <map>
#include <string>
#include <queue>

#include <glm/glm.hpp>

namespace Hudi {

	class Scene
	{
	public:
		Scene(uint8_t index);
		~Scene();
		uint8_t GetBuildIndex() const { return m_BuildIndex; }

		void BeginScene();
		void EndScene();

		void OnUpdate(float dt);
		
		void OnViewportResize(int width, int height);
		glm::vec2 GetViewportSize() const { return { m_Width, m_Height }; }

		Ref<Scene> Copy() const;

		void Flush();
		void Invalidate();

		// GameObject relevant
		GameObject CreateEmptyObject(const std::string& _name);
		GameObject CreateGameObject(const std::string& _name);

		bool HasGameObject(const std::string& _name) const;
		bool HasGameObject(GameObject object) const;

		GameObject GetGameObject(const std::string& _name) const;
		GameObject GetGameObject(uint32_t id) const;

		void DestroyGameObject(const std::string& _name);
		void DestroyGameObject(GameObject object);

		std::string GetGameObjectName(GameObject object) const;
		void RenameGameObject(const std::string& _name, GameObject object);

		bool SetPrimaryCamera(const std::string& cameraName);
		bool SetPrimaryCamera(GameObject camera);
		void ResetPrimaryCamera();
		GameObject GetPrimaryCamera() const;

		// Loop through all GameObjects
		template <typename Func>
		void Each(Func&& func);

	private:
		uint8_t m_BuildIndex = 0;
		int m_Width = 0;
		int m_Height = 0;
		
	private:
		std::map<uint32_t, GameObject> m_GameObjects;
		
		std::map<uint32_t, std::string> m_EntityToName;
		std::map<std::string, uint32_t> m_NameToEntity;

		std::queue<uint32_t> m_DestroyedObjects;

		GameObject m_PrimaryCamera;
	private:
		Ref<ECS::World> m_World;
		Ref<RenderSystem> m_RenderSystem;
		Ref<Physics2DSystem> m_Physics2DSystem;
	private:
		friend class SceneSerializer;
	};


	// --------- Impl ---------------------------------------------------------- //
	template <typename Func>
	inline void Scene::Each(Func&& func)
	{
		for (auto& [id, object] : m_GameObjects)
		{
			std::forward<Func>(func)(object);
		}
	}
}