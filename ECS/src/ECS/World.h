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

		void Flush();

	public:
		template <typename T>
		std::shared_ptr<T> RegisterSystem();

	public:
		Entity CreateEntity();
		void DestroyEntity(Entity entt) { s_DestroyedEntities.push(entt); }
		bool Exists(Entity entt) { return m_EntityManager->Exists(entt); }

		void SetActive(Entity entt, bool active);
		bool IsActive(Entity entt) { return m_EntityManager->IsActive(entt); }

	public:
		template <typename T, typename ... Args>
		std::shared_ptr<T> CreateComponent(Args&&... args);

		template <typename T>
		std::shared_ptr<T> AddComponent(Entity entt);

		template <typename T>
		std::shared_ptr<T> AddComponent(Entity emtt, std::shared_ptr<BaseComponent> comp);

		template <typename T>
		void AddSystem(Entity entt);

		template <typename T>
		bool HasComponent(Entity entt);

		template <typename T>
		std::shared_ptr<T> GetComponent(Entity entt) { return m_ComponentManager->GetComponent<T>(entt); }

		std::vector<std::shared_ptr<BaseComponent>> GetComponents(Entity entt) { return m_ComponentManager->GetComponents(entt); }

		template <typename T>
		void RemoveComponent(Entity entt);

		template <typename T>
		std::vector<Entity> View();

		// Function passed in must take a std::shared_ptr<Comp> as an argument
		template <typename Comp, typename Fn>
		void EachComponent(Fn&& func) { m_ComponentManager->EachComponent<Comp>(func); }

		// Function passed in must take a std::shared_ptr<ClientComponentType> as an argument
		template <typename ClientComponentType, typename Fn>
		void EachComponents(Fn&& func) { m_ComponentManager->EachComponents<ClientComponentType>(func); }

	private:
		std::unique_ptr<EntityManager> m_EntityManager;
		std::unique_ptr<ComponentManager> m_ComponentManager;
		std::unique_ptr<SystemManager> m_SystemManager;

		std::queue<Entity> s_DestroyedEntities;
	};
	// ----------------------------------------------------------------------------------------------------- //


	// Definitions
	inline World::World()
	{
		m_EntityManager = std::make_unique<EntityManager>();
		m_ComponentManager = std::make_unique<ComponentManager>();
		m_SystemManager = std::make_unique<SystemManager>();
	}

	template <typename T>
	inline std::shared_ptr<T> World::RegisterSystem()
	{
		std::shared_ptr<System> system = m_SystemManager->RegisterSystem<T>();
		system->world = this;
		return std::static_pointer_cast<T>(system);
	}

	inline void World::Flush()
	{
		while (!s_DestroyedEntities.empty())
		{
			Entity entt = s_DestroyedEntities.front();

			m_EntityManager->DestroyEntity(entt);
			m_ComponentManager->DestroyEntity(entt);
			m_SystemManager->DestroyEntity(entt);

			s_DestroyedEntities.pop();
		}
	}

	template <typename T, typename ... Args>
	std::shared_ptr<T> World::CreateComponent(Args&&... args)
	{
		std::shared_ptr<BaseComponent> component = std::make_shared<T>(std::forward<Args>(args)...);
		component->world = this;
		return std::static_pointer_cast<T>(component);
	}

	template <typename T>
	inline std::shared_ptr<T> World::AddComponent(Entity entt)
	{
		std::shared_ptr<BaseComponent> component = CreateComponent<T>();
		component->m_Entt = entt;

		m_ComponentManager->AddComponent<T>(entt, component);
		component->Init();

		ComponentID id = GetComponentID<T>();
		Signature& sig = m_EntityManager->GetComponentSignature(entt);
		sig.set(id);

		m_SystemManager->AddEntity(entt, sig);

		return std::static_pointer_cast<T>(component);
	}

	template <typename T>
	inline std::shared_ptr<T> World::AddComponent(Entity entt, std::shared_ptr<BaseComponent> component)
	{
		if (component->world != this)
		{
			std::cout << "ERROR: Adding a component that does not belong to the world!\n";
			return nullptr;
		}
		component->m_Entt = entt;

		m_ComponentManager->AddComponent<T>(entt, component);
		component->Init();

		ComponentID id = GetComponentID<T>();
		Signature& sig = m_EntityManager->GetComponentSignature(entt);
		sig.set(id);

		m_SystemManager->AddEntity(entt, sig);

		return std::static_pointer_cast<T>(component);
	}

	template <typename T>
	inline void World::AddSystem(Entity entt)
	{
		Signature compSig = m_EntityManager->GetComponentSignature(entt);
		m_SystemManager->AddSystem<T>(entt, compSig);
	}

	inline Entity World::CreateEntity()
	{
		Entity entt = m_EntityManager->CreateEntity();
		return entt;
	}

	template <typename T>
	inline bool World::HasComponent(Entity entt)
	{
		Signature CompSig;
		CompSig.set(GetComponentID<T>());

		return (CompSig & (m_EntityManager->GetComponentSignature(entt))) != 0;
	}

	template <typename T>
	inline void World::RemoveComponent(Entity entt)
	{
		auto& sig = m_EntityManager->GetComponentSignature(entt);
		ComponentID id = GetComponentID<T>();
		sig.set(id, false);
		m_ComponentManager->RemoveComponent<T>(entt);
		m_SystemManager->Invalidate(entt, sig);
	}

	template <typename T>
	inline std::vector<Entity> World::View()
	{
		return m_ComponentManager->View<T>();
	}

	inline void World::SetActive(Entity entt, bool active)
	{
		m_EntityManager->SetActive(entt, active);

		if (!active)
			m_SystemManager->DestroyEntity(entt);
		else
			m_SystemManager->AddEntity(entt, m_EntityManager->GetComponentSignature(entt));
	}

}