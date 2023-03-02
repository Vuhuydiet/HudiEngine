#pragma once

#include "ecspch.h"

#include "EntityManager.h"
#include "ComponentManager.h"

namespace ECS {

	class System 
	{
	public:
		virtual ~System() = default;

		Signature GetSignature() const { return m_Signature; }

		void AddEntity(Entity entt) { m_Entities.insert(entt); }
		void RemoveEntity(Entity entt) { m_Entities.erase(entt); }

		virtual void OnUpdate() = 0;

	protected:
		Signature m_Signature;
		std::set<Entity> m_Entities;

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
		~SystemManager()
		{
			for (auto& system : m_Systems)
			{
				delete system;
			}
		}

		template <typename T>
		T* RegisterSystem()
		{
			SystemID id = GetSystemID<T>();

			if (id==m_Size) 
			{
				m_Systems[id] = new T();
				m_Size++;
			}

			return static_cast<T*>(m_Systems[id]);
		}

		template <typename T>
		void AddSystem(Entity entt, const Signature& sig) 
		{
			SystemID id = GetSystemID<T>();

			System* system = m_Systems[id];

			Signature sysSig = system->GetSignature();

			if ((sig & sysSig) == sysSig)
			{
				system->AddEntity(entt);
			}
		}

		void AddEntity(Entity entt, const Signature& sig)
		{
			for (size_t id = 0; id<m_Size; id++)
			{
				auto& system = m_Systems[id];
				const auto& sysSig = system->GetSignature();
				if ((sysSig & sig) == sysSig)
				{
					system->AddEntity(entt);
				}
			}
		}

		void DestroyEntity(Entity entt) 
		{
			for (size_t id = 0; id < m_Size; id++)
			{
				m_Systems[id]->RemoveEntity(entt);
			}
		}

		template <typename T>
		void OnUpdate()
		{
			m_Systems[GetSystemID<T>()]->OnUpdate();
		}

	private:
		std::array<System*, MAX_SYSTEMS> m_Systems;
		size_t m_Size = 0;
	};

}