#pragma once

#include "ecspch.h"

namespace ECS {

	using Entity = int;
	const Entity MAX_ENTITIES = 5000;

	using ComponentID = int;
	const ComponentID MAX_COMPONENTS = 1000;

	using SystemID = int;
	const SystemID MAX_SYSTEMS = 32;

	using Signature = std::bitset<MAX_COMPONENTS>;


	//-----------------ENTITY----------------------//
	class EntityManager
	{
	public:

		EntityManager() 
		{
			for (int i = 1; i < MAX_ENTITIES; i++)
			{
				m_EntityQueue.push(i);
				m_Exists[i] = false;
			}
			m_Exists[0] = false;
		}

		Entity CreateEntity()
		{
			Entity newEntt = m_EntityQueue.front();
			m_EntityQueue.pop();

			m_Exists[newEntt] = true;

			return newEntt;
		}

		void DestroyEntity(Entity entt)
		{
			m_AvailableComponents[entt].reset();

			m_EntityQueue.push(entt);

			m_Exists[entt] = false;
		}

		bool Exist(Entity entt) { return m_Exists[entt]; }

		Signature& GetComponentSignature(Entity entt)
		{
			return m_AvailableComponents[entt];
		}

	private:
		std::queue<Entity> m_EntityQueue;

		std::array<Signature, MAX_ENTITIES> m_AvailableComponents;

		std::array<bool, MAX_ENTITIES> m_Exists;
	};

}