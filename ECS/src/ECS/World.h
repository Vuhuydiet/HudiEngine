#pragma once

#include <memory>
#include <queue>

#include "EntityManager.h"
#include "ComponentManager.h"
#include "SystemManager.h"

namespace ECS {

	class World
	{
	public:
		World();
		void Init();

		void OnUpdate();

	public:
		template <typename T>
		T* RegisterSystem() { return s_SystemManager->RegisterSystem<T>(); }

	public:
		Entity CreateEntity();
		void DestroyEntity(Entity entt) { s_DestroyedEntities.push(entt); }
		bool Exists(Entity entt) { return s_EntityManager->Exists(entt); }

		void SetActive(Entity entt, bool active);
		bool IsActive(Entity entt) { return s_EntityManager->IsActive(entt); }

	public:
		template <typename T>
		std::shared_ptr<T> CreateComponent();

		template <typename T>
		std::shared_ptr<T> AddComponent(Entity entt);

		template <typename T>
		std::shared_ptr<T> AddComponent(Entity emtt, std::shared_ptr<Component> comp);

		template <typename T>
		bool HasComponent(Entity entt);

		template <typename T>
		std::shared_ptr<T> GetComponent(Entity entt) { return s_ComponentManager->GetComponent<T>(entt); }

		std::vector<std::shared_ptr<Component>> GetComponents(Entity entt) { return s_ComponentManager->GetComponents(entt); }

		template <typename T>
		void AddSystem(Entity entt);

		template <typename T, typename Fn>
		void Each(Fn func) { s_ComponentManager->Each<T>(func); }

	private:
		std::unique_ptr<EntityManager> s_EntityManager;
		std::unique_ptr<ComponentManager> s_ComponentManager;
		std::unique_ptr<SystemManager> s_SystemManager;

		std::queue<Entity> s_DestroyedEntities;
	};
	// ----------------------------------------------------------------------------------------------------- //


	// Definitions
	inline World::World()
	{
		s_EntityManager = std::make_unique<EntityManager>();
		s_ComponentManager = std::make_unique<ComponentManager>();
		s_SystemManager = std::make_unique<SystemManager>();
	}

	inline void World::OnUpdate()
	{
		while (!s_DestroyedEntities.empty())
		{
			Entity entt = s_DestroyedEntities.front();

			s_EntityManager->DestroyEntity(entt);
			s_ComponentManager->DestroyEntity(entt);
			s_SystemManager->DestroyEntity(entt);

			s_DestroyedEntities.pop();
		}
	}

	template <typename T> 
	std::shared_ptr<T> World::CreateComponent()
	{
		std::shared_ptr<Component> component = std::make_shared<T>();
		component->world = this;
		return std::static_pointer_cast<T>(component);
	}

	template <typename T>
	inline std::shared_ptr<T> World::AddComponent(Entity entt)
	{
		std::shared_ptr<Component> component = CreateComponent<T>();
		component->m_Entt = entt;

		s_ComponentManager->AddComponent<T>(entt, component);
		component->Init();

		ComponentID id = GetComponentID<T>();
		Signature& sig = s_EntityManager->GetComponentSignature(entt);
		sig.set(id);

		s_SystemManager->AddEntity(entt, sig);

		return std::static_pointer_cast<T>(component);
	}

	template <typename T>
	inline std::shared_ptr<T> World::AddComponent(Entity entt, std::shared_ptr<Component> component) 
	{
		if (component->world != this)
		{
			std::cout << "ERROR: Adding a component that does not belong to the world!\n";
			return nullptr;
		}
		component->m_Entt = entt;

		s_ComponentManager->AddComponent<T>(entt, component);
		component->Init();

		ComponentID id = GetComponentID<T>();
		Signature& sig = s_EntityManager->GetComponentSignature(entt);
		sig.set(id);

		s_SystemManager->AddEntity(entt, sig);

		return std::static_pointer_cast<T>(component);
	}

	template <typename T>
	inline void World::AddSystem(Entity entt)
	{
		Signature compSig = s_EntityManager->GetComponentSignature(entt);
		s_SystemManager->AddSystem<T>(entt, compSig);
	}

	inline Entity World::CreateEntity()
	{
		Entity entt = s_EntityManager->CreateEntity();
		return entt;
	}

	template <typename T>
	inline bool World::HasComponent(Entity entt)
	{
		Signature CompSig;
		CompSig.set(GetComponentID<T>());

		return (CompSig & (s_EntityManager->GetComponentSignature(entt))) != 0;
	}

	inline void World::SetActive(Entity entt, bool active)
	{
		s_EntityManager->SetActive(entt, active);

		if (!active)
			s_SystemManager->DestroyEntity(entt);
		else
			s_SystemManager->AddEntity(entt, s_EntityManager->GetComponentSignature(entt));
	}

}