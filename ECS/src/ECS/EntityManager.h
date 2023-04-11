#pragma once

#include <queue>
#include <array>
#include <bitset>

namespace ECS {

	using Entity = uint32_t;
	const Entity MAX_ENTITIES = 10000;

	using ComponentID = uint32_t;
	const ComponentID MAX_COMPONENTS = 1000;

	using SystemID = uint8_t;
	const SystemID MAX_SYSTEMS = 32;

	using Signature = std::bitset<MAX_COMPONENTS>;


	//----------------- ENTITY ----------------------//
	class EntityManager
	{
	public:

		EntityManager() 
		{
			for (int i = 1; i < MAX_ENTITIES; i++)
			{
				m_EntityQueue.push(i);
				m_Exists[i] = false;
				m_IsActives[i] = false;
			}
			m_Exists[0] = false;
			m_IsActives[0] = false;
		}

		Entity CreateEntity()
		{
			if (m_EntityQueue.empty())
			{
				std::cout << "WARNING: Entity capacity maximum is " << MAX_ENTITIES << "at a time!\n";
				return 0;
			}

			Entity newEntt = m_EntityQueue.front();
			m_EntityQueue.pop();

			m_Exists[newEntt] = true;
			m_IsActives[newEntt] = true;

			return newEntt;
		}

		void DestroyEntity(Entity entt)
		{
			m_AvailableComponents[entt].reset();
			m_EntityQueue.push(entt);

			m_Exists[entt] = false;
			m_IsActives[entt] = false;
		}

		bool Exists(Entity entt) { return m_Exists[entt]; }

		Signature& GetComponentSignature(Entity entt) { return m_AvailableComponents[entt]; }

		void SetActive(Entity entt, bool active) { m_IsActives[entt] = active; }
		bool IsActive(Entity entt) { return m_IsActives[entt]; }

	private:
		std::queue<Entity> m_EntityQueue;
		std::array<Signature, MAX_ENTITIES> m_AvailableComponents;

		std::array<bool, MAX_ENTITIES> m_Exists;
		std::array<bool, MAX_ENTITIES> m_IsActives;
	};

}