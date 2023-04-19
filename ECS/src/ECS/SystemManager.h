#pragma once

#include <unordered_set>

#include "EntityManager.h"
#include "ComponentManager.h"

namespace ECS {

	class World;

	class System 
	{
	public:
		virtual ~System() = default;

		Signature GetSignature() const { return m_Signature; }

		void AddEntity(Entity entt) { m_Entities.insert(entt); }
		void RemoveEntity(Entity entt) { m_Entities.erase(entt); }

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
		std::shared_ptr<T> RegisterSystem();

		template <typename T>
		void AddSystem(Entity entt, const Signature& sig);

		void AddEntity(Entity entt, const Signature& sig);

		void Invalidate(Entity entt, const Signature& sig);
		
		void DestroyEntity(Entity entt);

	private:
		std::array<std::shared_ptr<System>, MAX_SYSTEMS> m_Systems;
		size_t m_Size = 0;
	};



	// ------------------ Definitions --------------------------------------//
	template <typename T>
	inline std::shared_ptr<T> SystemManager::RegisterSystem()
	{
		SystemID id = GetSystemID<T>();

		if (id == m_Size)
		{
			m_Systems[id] = std::make_shared<T>();
			m_Size++;
		}

		return std::static_pointer_cast<T>(m_Systems[id]);
	}

	template <typename T>
	inline void SystemManager::AddSystem(Entity entt, const Signature& sig)
	{
		SystemID id = GetSystemID<T>();
		System* system = m_Systems[id];
		Signature sysSig = system->GetSignature();

		if ((sig & sysSig) == sysSig)
		{
			system->AddEntity(entt);
		}
	}

	inline void SystemManager::AddEntity(Entity entt, const Signature& sig)
	{
		for (size_t id = 0; id < m_Size; id++)
		{
			auto& system = m_Systems[id];
			const auto& sysSig = system->GetSignature();
			if ((sysSig & sig) == sysSig)
			{
				system->AddEntity(entt);
			}
		}
	}

	inline void SystemManager::Invalidate(Entity entt, const Signature& sig)
	{
		for (size_t id = 0; id < m_Size; id++)
		{
			auto& system = m_Systems[id];
			const auto& sysSig = system->GetSignature();
			if ((sysSig & sig) != sysSig)
			{
				system->RemoveEntity(entt);
			}
		}
	}

	inline void SystemManager::DestroyEntity(Entity entt)
	{
		for (size_t id = 0; id < m_Size; id++)
		{
			m_Systems[id]->RemoveEntity(entt);
		}
	}
}