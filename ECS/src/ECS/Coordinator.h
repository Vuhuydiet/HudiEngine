#pragma once
#include "ecspch.h"

#include "EntityManager.h"
#include "ComponentManager.h"
#include "SystemManager.h"

namespace ECS {

	class Coordinator
	{
	public:
		Coordinator() = delete;
		Coordinator(const Coordinator&) = delete;

		static void Init();

		static void OnUpdate();

	public:
		template <typename T>
		static T* RegisterSystem() { return s_SystemManager->RegisterSystem<T>(); }

	public:
		template <typename T>
		static T& AddComponent(Entity entt, std::shared_ptr<Component> component);

		template <typename T>
		static void AddSystem(Entity entt);

	public:
		static Entity CreateEntity();
		static void DestroyEntity(Entity entt) { s_DestroyedEntities.push(entt); }

		template <typename T>
		static bool HasComponent(Entity entt);

		static bool Exist(Entity entt) { return s_EntityManager->Exist(entt); }

		template <typename T>
		static T& GetComponent(Entity entt) { return s_ComponentManager->GetComponent<T>(entt); }

		static std::vector<std::shared_ptr<Component>> GetComponents(Entity entt) { return s_ComponentManager->GetComponents(entt); }

		static void SetActive(Entity entt, bool active);
		static bool IsActive(Entity entt) { return s_ComponentManager->IsActive(entt); }


		static void OnAwakeComponents() { s_ComponentManager->OnAwakeComponents(); }

		static void OnUpdateComponents() { s_ComponentManager->OnUpdateComponents(); }

		template <typename T>
		static void OnUpdateSystem() { s_SystemManager->OnUpdate<T>(); }

	private:
		static std::unique_ptr<EntityManager> s_EntityManager;
		static std::unique_ptr<ComponentManager> s_ComponentManager;
		static std::unique_ptr<SystemManager> s_SystemManager;

		static std::queue<Entity> s_DestroyedEntities;
	};
	// ----------------------------------------------------------------------------------------------------- //



	// Definitions
	inline void Coordinator::Init()
	{
		s_EntityManager = std::make_unique<EntityManager>();
		s_ComponentManager = std::make_unique<ComponentManager>();
		s_SystemManager = std::make_unique<SystemManager>();
	}

	inline void Coordinator::OnUpdate()
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
	inline T& Coordinator::AddComponent(Entity entt, std::shared_ptr<Component> component)
	{
		ComponentID id = GetComponentID<T>();
		Signature& sig = s_EntityManager->GetComponentSignature(entt);
		sig.set(id);

		component->m_Entt = entt;

		s_ComponentManager->AddComponent<T>(entt, component);
		component->Init();

		s_SystemManager->AddEntity(entt, sig);

		return *std::static_pointer_cast<T>(component);
	}

	template <typename T>
	inline void Coordinator::AddSystem(Entity entt)
	{
		Signature compSig = s_EntityManager->GetComponentSignature(entt);
		s_SystemManager->AddSystem<T>(entt, compSig);
	}

	inline Entity Coordinator::CreateEntity()
	{
		Entity entt = s_EntityManager->CreateEntity();
		SetActive(entt, true);
		return entt;
	}

	template <typename T>
	inline bool Coordinator::HasComponent(Entity entt)
	{
		Signature CompSig;
		CompSig.set(GetComponentID<T>());

		return (CompSig & (s_EntityManager->GetComponentSignature(entt))) != 0;
	}

	inline void Coordinator::SetActive(Entity entt, bool active)
	{
		s_ComponentManager->SetActive(entt, active);

		if (!active)
			s_SystemManager->DestroyEntity(entt);
		else
			s_SystemManager->AddEntity(entt, s_EntityManager->GetComponentSignature(entt));
	}

}