#pragma once

#include <vector>
#include <unordered_map>

#include "EntityManager.h"

#include "Component.h"

namespace ECS {

	inline ComponentID GetComponentID()
	{
		static ComponentID ID = 0;
		return ID++;
	}

	template <typename T>
	inline ComponentID GetComponentID() noexcept
	{
		static ComponentID id = GetComponentID();
		return id;
	}

	//----------Component Manager-----------------//
	class ComponentManager
	{
	public:
		template <typename T>
		void AddComponent(Entity entt, std::shared_ptr<Component> component);

		template <typename T>
		std::shared_ptr<T> GetComponent(Entity entt);

		std::vector<std::shared_ptr<Component>> GetComponents(Entity entt);

		template <typename T>
		void RemoveComponent(Entity entt);

		void DestroyEntity(Entity entt);

		template <typename Comp, typename Fn>
		void EachInComponentType(Fn func);

		template <typename T, typename Fn>
		void Each(Fn func);

	private:
		std::unordered_map<Entity, std::shared_ptr<Component>> m_ComponentMaps[MAX_COMPONENTS];
		size_t m_Size = 0;

	};


	// --------------- Definitions -------------------------------- //

	template <typename T>
	inline void ComponentManager::AddComponent(Entity entt, std::shared_ptr<Component> component)
	{
		ComponentID id = GetComponentID<T>();

		m_Size = std::max(m_Size, (size_t)id + 1);

		m_ComponentMaps[id][entt] = component;
	}

	template <typename T>
	inline std::shared_ptr<T> ComponentManager::GetComponent(Entity entt)
	{
		ComponentID id = GetComponentID<T>();

		if (m_ComponentMaps[id].find(entt) == m_ComponentMaps[id].end())
		{
			std::cout << "This entity " << entt << " has no component of type \"" << T()
				<< "\". Returning nullptr \n";
			return nullptr;
		}
		return std::static_pointer_cast<T>(m_ComponentMaps[id][entt]);
	}

	inline std::vector<std::shared_ptr<Component>> ComponentManager::GetComponents(Entity entt)
	{
		std::vector<std::shared_ptr<Component>> components;
		for (int id = 0; id < m_Size; id++)
		{
			if (m_ComponentMaps[id].find(entt) != m_ComponentMaps[id].end())
				components.push_back(m_ComponentMaps[id][entt]);
		}
		return components;
	}

	template <typename T>
	inline void ComponentManager::RemoveComponent(Entity entt)
	{
		ComponentID id = GetComponentID<T>();
		m_ComponentMaps[id].erase(entt);
	}

	inline void ComponentManager::DestroyEntity(Entity entt)
	{
		for (ComponentID id = 0; id < m_Size; id++)
		{
			if (m_ComponentMaps[id].find(entt) != m_ComponentMaps[id].end())
				m_ComponentMaps[id].erase(entt);
		}
	}

#ifdef CALL_FOR_EACH_COMPONENT_TYPE
	template <typename Comp, typename Fn>
	inline void ComponentManager::EachInComponentType(Fn func)
	{
		ComponentID id = GetComponentID<Comp>();
		for (auto& [entt, comp] : m_ComponentMaps[id])
		{
			if (!comp->enabled)
				continue;
			std::bind(func, (Comp*)(comp.get()))();
		}
	}
#endif

	template <typename T, typename Fn>
	inline void ComponentManager::Each(Fn func)
	{
		for (int i = 0; i < m_Size; i++)
		{
			for (auto& [entt, comp] : m_ComponentMaps[i])
			{
				if (!comp->enabled)
					continue;
				std::bind(func, (T*)comp.get())();
			}
		}
	}
}