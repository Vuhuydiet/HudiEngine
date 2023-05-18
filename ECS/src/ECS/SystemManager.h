#pragma once

#include <unordered_set>
#include <map>
#include <type_traits>

#include "EntityManager.h"
#include "ComponentManager.h"

namespace ECS {

	class World;

	class System 
	{
	public:
		virtual ~System() = default;

		template <typename T, typename ... Args>
		void SetNeededComponent();
		Signature GetSignature() const { return m_Signature; }

		void AddEntity(Entity entt);
		void RemoveEntity(Entity entt);
		bool HasEntity(Entity entt) const;

		virtual void OnEntityAdded(Entity entity) {}
		virtual void OnEntityRemoved(Entity entity) {}

	private:
		template <typename ... Args>
		typename decltype(std::enable_if<sizeof...(Args)==0>::type()) SetNeededComponent() {}

	protected:
		Signature m_Signature;
		std::unordered_set<Entity> m_Entities;

		World* world = nullptr;
		friend class World;
	};

	//---------------(Get System ID)-----------------//
	inline SystemID GetSystemID()
	{
		static SystemID id = 0;
		return id++;
	}

	template <typename T>
	inline SystemID GetSystemID() noexcept
	{
		static SystemID id = GetSystemID();
		return id;
	}

	//-----------------System manager----------------//
	class SystemManager 
	{
	public:
		template <typename T>
		std::shared_ptr<System> RegisterSystem();

		template <typename T>
		void AddSystem(Entity entt, const Signature& sig);

		void AddEntity(Entity entt, const Signature& sig);

		void Invalidate(Entity entt, const Signature& sig);
		
		void DestroyEntity(Entity entt);

	private:
		std::map<SystemID, std::shared_ptr<System>> m_Systems;
		// size_t m_Size = 0;
	};



	// ------------------ Definitions --------------------------------------//

	template <typename T, typename ... Args>
	inline void System::SetNeededComponent()
	{
		m_Signature.set(GetComponentID<T>());
		SetNeededComponent<Args...>();
	}

	inline void System::AddEntity(ECS::Entity entity)
	{
		OnEntityAdded(entity);
		m_Entities.insert(entity);
	}

	inline void System::RemoveEntity(ECS::Entity entity)
	{
		OnEntityRemoved(entity);
		m_Entities.erase(entity);
	}

	inline bool System::HasEntity(Entity entity) const
	{
		return m_Entities.find(entity) != m_Entities.end();
	}

	//////////////////////////////////////////////////////////////////////////////////
	////////// System Manager ////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////////

	template <typename T>
	inline std::shared_ptr<System> SystemManager::RegisterSystem()
	{
		SystemID id = GetSystemID<T>();
		if (m_Systems.find(id) == m_Systems.end())
		{
			m_Systems[id] = std::make_shared<T>();
			//m_Size++;
		}

		return m_Systems.at(id);
	}

	template <typename T>
	inline void SystemManager::AddSystem(Entity entt, const Signature& sig)
	{
		SystemID id = GetSystemID<T>();
		System* system = m_Systems.at(id);
		Signature sysSig = system->GetSignature();

		if ((sig & sysSig) == sysSig && !system->HasEntity(entt))
		{
			system->AddEntity(entt);
		}
	}

	inline void SystemManager::AddEntity(Entity entt, const Signature& sig)
	{
		for (auto& [systemID, system] : m_Systems)
		{
			const auto& sysSig = system->GetSignature();
			if ((sysSig & sig) == sysSig && !system->HasEntity(entt))
			{
				system->AddEntity(entt);
			}
		}
	}

	inline void SystemManager::Invalidate(Entity entt, const Signature& sig)
	{
		for (auto& [systemID, system] : m_Systems)
		{
			const auto& sysSig = system->GetSignature();
			if ((sysSig & sig) != sysSig && system->HasEntity(entt))
			{
				system->RemoveEntity(entt);
			}
		}
	}

	inline void SystemManager::DestroyEntity(Entity entity)
	{
		for (auto& [systemID, system] : m_Systems)
		{
			if (system->HasEntity(entity))
				system->RemoveEntity(entity);
		}
	}
}