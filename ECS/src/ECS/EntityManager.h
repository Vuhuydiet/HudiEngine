#pragma once

#include <queue>
#include <array>
#include <bitset>
#include <iostream>

namespace ECS {

	using Entity = uint32_t;
	const uint32_t MAX_ENTITIES = 20000;

	using ComponentID = uint32_t;
	const uint32_t MAX_COMPONENTS = 1000;

	using SystemID = uint8_t;
	const uint32_t MAX_SYSTEMS = 32;

	using Signature = std::bitset<MAX_COMPONENTS>;

	const Entity null = 0;


	//----------------- ENTITY ----------------------//
	class EntityManager
	{
	public:

		EntityManager();

		Entity CreateEntity();
		void DestroyEntity(Entity entt);

		Signature& GetComponentSignature(Entity entt) { return m_AvailableComponents[entt]; }

		std::vector<Entity> GetActiveEntities();
		bool Exists(Entity entt) { return m_Exists[entt]; }

		void SetActive(Entity entt, bool active) { m_IsActives[entt] = active; }
		bool IsActive(Entity entt) { return m_IsActives[entt]; }

	private:
		std::queue<Entity> m_EntityQueue;
		std::array<Signature, MAX_ENTITIES> m_AvailableComponents;

		std::array<bool, MAX_ENTITIES> m_Exists;
		std::array<bool, MAX_ENTITIES> m_IsActives;
	};



	// ------------- Definitions --------------------------------------------------------//
	inline EntityManager::EntityManager()
	{
		for (int i = 0; i < MAX_ENTITIES; i++)
		{
			m_Exists[i] = false;
			m_IsActives[i] = false;
			if (i != 0)
				m_EntityQueue.push(i);
		}
	}

	inline Entity EntityManager::CreateEntity()
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

	inline void EntityManager::DestroyEntity(Entity entt)
	{
		m_AvailableComponents[entt].reset();
		m_EntityQueue.push(entt);

		m_Exists[entt] = false;
		m_IsActives[entt] = false;
	}

}