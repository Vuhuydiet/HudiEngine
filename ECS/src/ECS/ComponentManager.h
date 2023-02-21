#pragma once
#include "ecspch.h"

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
		void AddComponent(Entity entt, std::shared_ptr<Component> component)
		{
			ComponentID id = GetComponentID<T>();

			m_Size = std::max(m_Size, (size_t)id+1);

			m_ComponentMaps[id][entt] = component;
		}

		template <typename T>
		T& GetComponent(Entity entt)
		{
			ComponentID id = GetComponentID<T>();

			if (m_ComponentMaps[id].find(entt) == m_ComponentMaps[id].end())
			{
				std::cout << "This entity " << entt << " has no component of type \"" << T() 
					<< "\". Returning default \"" << T() << "\" \n";
				static T t;
				return t;
			}
			return *std::static_pointer_cast<T>(m_ComponentMaps[id][entt]);
		}

		std::vector<std::shared_ptr<Component>> GetComponents(Entity entt)
		{
			std::vector<std::shared_ptr<Component>> components;
			for (int id = 0; id < m_Size; id++)
			{
				if (m_ComponentMaps[id].find(entt)!=m_ComponentMaps[id].end())
					components.push_back(m_ComponentMaps[id][entt]);
			}
			return components;
		}

		template <typename T>
		void RemoveComponent(Entity entt)
		{
			ComponentID id = GetComponentID<T>();
			m_ComponentMaps[id].erase(entt);
		}

		void DestroyEntity(Entity entt)
		{
			for (ComponentID id = 0; id < m_Size; id++)
			{
				if (m_ComponentMaps[id].find(entt)!=m_ComponentMaps[id].end())
					m_ComponentMaps[id].erase(entt);
			}
			m_IsActives.erase(entt);
		}

		void SetActive(Entity entt, bool active) { m_IsActives[entt] = active; }
		bool IsActive(Entity entt) { return m_IsActives[entt]; }
		

		void OnAwake(ComponentID id)
		{
			if (!IsAwakable(id))
				return;

			for (auto& [entt, comp] : m_ComponentMaps[id])
			{
				if (!IsActive(entt))
					continue;

				if (comp->enabled)
					comp->Awake();
			}
		}

		void OnAwakeComponents()
		{
			for (ComponentID id = 0; id < m_Size; id++)
			{
				if (IsInOrder(id, "first"))
					OnAwake(id);
			}
			for (ComponentID id = 0; id < m_Size; id++)
			{
				if ((IsInOrder(id, "second")))
					OnAwake(id);
			}
			for (ComponentID id = 0; id < m_Size; id++)
			{
				if ((IsInOrder(id, "last")))
					OnAwake(id);
			}
		}

		void OnUpdate(ComponentID id)
		{
			if (!IsUpdatable(id))
				return;
			
			for (auto& [entt, comp] : m_ComponentMaps[id])
			{
				if (!IsActive(entt))
					continue;

				if (comp->enabled)
					comp->Update();
			}
		}

		void OnUpdateComponents()
		{
			for (ComponentID id = 0; id < m_Size; id++)
			{
				if (IsInOrder(id, "first"))
					OnUpdate(id);
			}
			for (ComponentID id = 0; id < m_Size; id++)
			{
				if (IsInOrder(id, "second"))
				{
					OnUpdate(id);
				}
			}
			for (ComponentID id = 0; id < m_Size; id++)
			{
				if (IsInOrder(id, "last"))
				{
					OnUpdate(id);
				}
			}
		}

	private:
		bool IsInOrder(ComponentID id, const char* order)
		{
			if (m_ComponentMaps[id].size() == 0)
				return false;
			return m_ComponentMaps[id].begin()->second->OrderOfUpdate() == order;
		}
		bool IsAwakable(ComponentID id)
		{
			if (m_ComponentMaps[id].size() == 0)
				return false;
			return m_ComponentMaps[id].begin()->second->IsAwakable();
		}
		bool IsUpdatable(ComponentID id)
		{
			if (m_ComponentMaps[id].size() == 0)
				return false;
			return m_ComponentMaps[id].begin()->second->IsUpdatable();
		}

	private:
		std::unordered_map<Entity, std::shared_ptr<Component>> m_ComponentMaps[MAX_COMPONENTS];
		std::unordered_map<Entity, bool> m_IsActives;

		size_t m_Size = 0;
	};

}