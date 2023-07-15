#pragma once

#include <memory>
#include <queue>
#include <type_traits>

#include "EntityManager.h"
#include "ComponentManager.h"
#include "SystemManager.h"

namespace ECS {

	class World
	{
	public:
		World();

	public:
		template <typename T>
		std::shared_ptr<T> RegisterSystem();

	public:
		Entity CreateEntity();
		void DestroyEntity(Entity entt) { m_DestroyedEntities.push(entt); }
		bool Exists(Entity entt) { return m_EntityManager->Exists(entt); }

		void SetActive(Entity entt, bool active);
		bool IsActive(Entity entt) { return m_EntityManager->IsActive(entt); }

		void Flush();

	public:
		template <typename T, typename ... Args>
		T* AddComponent(Entity entt, Args&&... args);

		template <typename T>
		bool HasComponent(Entity entt);

		template <typename T>
		T* GetComponent(Entity entt) { return m_ComponentManager->GetComponent<T>(entt); }
#ifdef SUPPORTED
		std::vector<void*> GetComponents(Entity entt) { return m_ComponentManager->GetComponents(entt); }
#endif
		template <typename T>
		void RemoveComponent(Entity entt);

		template <typename T, typename... Args>
		std::vector<Entity> View();

		// Loop through all components and call 'func' on it
		// 'func' must take a T* as its only argument
		template <typename T, typename Fn>
		void EachComponent(Fn&& func) { m_ComponentManager->EachComponent<T>(func); }

		template <typename T>
		void AddSystem(Entity entt);

	private:
		std::unique_ptr<EntityManager> m_EntityManager;
		std::unique_ptr<ComponentManager> m_ComponentManager;
		std::unique_ptr<SystemManager> m_SystemManager;

		std::queue<Entity> m_DestroyedEntities;
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
		while (!m_DestroyedEntities.empty())
		{
			Entity entt = m_DestroyedEntities.front();
			m_DestroyedEntities.pop();

			m_SystemManager->DestroyEntity(entt);
			m_ComponentManager->DestroyEntity(entt);
			m_EntityManager->DestroyEntity(entt);
		}
	}

	template <typename T, typename ... Args>
	inline T* World::AddComponent(Entity entt, Args&&... args)
	{
		//printf("Added %s, id: %llu\n", typeid(T).name(), GetComponentID<T>());
		if (HasComponent<T>(entt))
		{
			printf("Entity already has a component of type '%s'!\nCannot add 2 component of the same type.\nReturning old '%s' component.\n", typeid(T).name(), typeid(T).name());
			//fflush(stdin);
			//std::cout << "Entity already has a component of type '" << typeid(T).name() << "'!\nCannot add 2 component of the same type.\nReturning old '";
			return m_ComponentManager->GetComponent<T>(entt);
		}
		
		T* component = m_ComponentManager->AddComponent<T>(entt, std::forward<Args>(args)...);

		ComponentID id = GetComponentID<T>();
		Signature& sig = m_EntityManager->GetComponentSignature(entt);
		sig.set(id);

		m_SystemManager->AddEntity(entt, sig);

		return component;
	}

#ifdef ECS_ALLOW_ADD_COMPONENT_BY_REF
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
#endif

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
		return (CompSig & m_EntityManager->GetComponentSignature(entt)) != Signature::null();
	}

	template <typename T>
	inline void World::RemoveComponent(Entity entt)
	{
		auto& sig = m_EntityManager->GetComponentSignature(entt);
		ComponentID id = GetComponentID<T>();
		sig.set(id, false);
		m_SystemManager->Invalidate(entt, sig);
		m_ComponentManager->RemoveComponent<T>(entt);
	}

	template <typename... Args>
	typename decltype(std::enable_if<sizeof...(Args) == 0>::type()) ViewHelper(World* world, std::vector<Entity>& entities) {}
	template <typename T, typename... Args>
	void ViewHelper(World* world, std::vector<Entity>& entities)
	{
		for (auto& entity : entities)
		{
			if (entity != null && !world->HasComponent<T>())
				entity = null;
		}
		ViewHelper<Args...>();
	}
	template <typename T, typename... Args>
	std::vector<Entity> World::View()
	{
		std::vector<Entity> temp = m_ComponentManager->View<T>();
		ViewHelper<Args...>(this, temp);

		std::vector<Entity> res;
		for (const auto& entity : temp)
		{
			if (entity != 0)
				res.push_back(entity);
		}
		return res;
	}

	inline void World::SetActive(Entity entt, bool active)
	{
		m_EntityManager->SetActive(entt, active);

		/*if (!active)
			m_SystemManager->DestroyEntity(entt);
		else
			m_SystemManager->AddEntity(entt, m_EntityManager->GetComponentSignature(entt));
		*/
	}

}
