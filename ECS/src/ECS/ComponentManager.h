#pragma once

#include <vector>
#include <unordered_map>
#include <type_traits>
#include <utility>
#include <functional>
#include <typeinfo>

#include "EntityManager.h"

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

	//struct BaseComponent 
	//{
	//	virtual void Delete() = 0;
	//};

	//template <typename T>
	//struct ComponentT : public BaseComponent
	//{
	//	T* data;
	//	virtual void Delete() override { delete data; }
	//};

	struct Component
	{
		void* data = nullptr;
		std::function<void(void*)> deleter = nullptr;

		void Delete()
		{
			deleter(data);
			data = nullptr;
		}
	};

	//----------Component Manager-----------------//
	class ComponentManager
	{
	public:
		template <typename T>
		T* AddComponent(Entity entt, Component component);

		template <typename T>
		T* GetComponent(Entity entt);

		std::vector<void*> GetComponents(Entity entt);

		template <typename T>
		void RemoveComponent(Entity entt);

		void DestroyEntity(Entity entt);
#define OLD_VIEW 1
#if OLD_VIEW
		template <typename T>
		std::vector<Entity> View();
#endif
		// Function passed in must take a Base* as an argument
		template <typename Comp, typename Fn>
		void EachComponent(Fn&& func);

	private:
		std::unordered_map<Entity, Component> m_ComponentMaps[MAX_COMPONENTS];
		size_t m_Size = 0;

	};


	// --------------- Definitions -------------------------------- //

	template <typename T>
	inline T* ComponentManager::AddComponent(Entity entt, Component p)
	{
		ComponentID id = GetComponentID<T>();
		m_Size = std::max(m_Size, (size_t)id + 1);
		m_ComponentMaps[id][entt] = p;
		return (T*)p.data;
	}

	template <typename T>
	inline T* ComponentManager::GetComponent(Entity entt)
	{
		ComponentID id = GetComponentID<T>();
		if (m_ComponentMaps[id].find(entt) == m_ComponentMaps[id].end())
		{
			std::cout << "This entity " << entt << " has no component of type '" << typeid(T).name() << "'. Returning nullptr \n";
			return nullptr;
		}
		return (T*)m_ComponentMaps[id].at(entt).data;
	}

	inline std::vector<void*> ComponentManager::GetComponents(Entity entt)
	{
		std::vector<void*> components;
		for (int id = 0; id < m_Size; id++)
		{
			if (m_ComponentMaps[id].find(entt) != m_ComponentMaps[id].end())
				components.push_back(m_ComponentMaps[id].at(entt).data);
		}
		return components;
	}

	template <typename T>
	inline void ComponentManager::RemoveComponent(Entity entt)
	{
		ComponentID id = GetComponentID<T>();
		m_ComponentMaps[id].at(entt).Delete();
		m_ComponentMaps[id].erase(entt);
	}

	inline void ComponentManager::DestroyEntity(Entity entt)
	{
		for (ComponentID id = 0; id < m_Size; id++)
		{
			if (m_ComponentMaps[id].find(entt) == m_ComponentMaps[id].end())
				continue;
			m_ComponentMaps[id].at(entt).Delete();
			m_ComponentMaps[id].erase(entt);
		}
	}

#if OLD_VIEW
	template <typename T>
	inline std::vector<Entity> ComponentManager::View()
	{
		std::vector<Entity> entities;
		ComponentID id = GetComponentID<T>();
		for (const auto& [entt, comp] : m_ComponentMaps[id])
		{
			entities.push_back(entt);
		}
		return entities;
	}
#endif

	template <typename Comp, typename Fn>
	inline void ComponentManager::EachComponent(Fn&& func)
	{
		ComponentID id = GetCompnentID<Comp>();
		for (auto& [entt, comp] : m_ComponentMaps[id])
		{
			//std::bind(td::forward(func), (ClientComponentType*)comp.get())();
			std::forward<Fn>(func)((Comp*)comp.data);
		}
	}
}