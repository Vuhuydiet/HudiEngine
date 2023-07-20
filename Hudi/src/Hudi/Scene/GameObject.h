#pragma once

#include <vector>
#include <filesystem>

#include "Components/AllComponents.h"

namespace Hudi {

	class Scene;

	class GameObject
	{
	public:
		GameObject();
		explicit GameObject(ECS::World* _world);

		void Destroy();
		void Reset();

		// WARN: Copy GameObject but does not copy the ID Component
		void CopyComponents(const GameObject& srcObj);
		bool IsValid() const { return m_Entity != ECS::null && world != nullptr; }

		ECS::Entity GetEntityID() const { return m_Entity; }
		UUID GetUUID() const { return GetComponent<IDComponent>().ID; }

		bool operator== (const GameObject& other) const;
		bool operator!= (const GameObject& other) const;

		// ------- System -------/
		bool Exist() const { return world->Exists(m_Entity); }

		template <typename T, typename ... Args>
		T& AddComponent(Args&&... args) const { return *world->AddComponent<T>(m_Entity, std::forward<Args>(args)...); }

		template <typename T>
		bool HasComponent() const { return world->HasComponent<T>(m_Entity); }

		template <typename T>
		T& GetComponent() const { return *world->GetComponent<T>(m_Entity); }

		template <typename T>
		T& GetOrAddComponent() const { return HasComponent<T>() ? GetComponent<T>() : AddComponent<T>(); }

#ifdef ECS_ALLOW_ADD_COMPONENT_BY_REF
		std::vector<void*> GetComponents() const;
#endif
		template <typename T>
		void RemoveComponent() const { world->RemoveComponent<T>(m_Entity); }

		template <typename T> 
		void AddSystem() const { world->AddSystem<T>(m_Entity); }

		void SetActive(bool active) const { world->SetActive(m_Entity, active); }
		bool IsActive() const { return world->IsActive(m_Entity); }

	private:
		ECS::Entity m_Entity;

		std::vector<std::filesystem::path> m_ScriptPaths;

	private:
		ECS::World* world;
		friend class Scene;
	};

}

