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

	struct Component
	{
		virtual ~Component() = default;
		virtual void* GetData() const = 0;
	};

	template <typename T>
	struct TComponent : public Component
	{
		T* data = nullptr;

		template <typename... Args>
		TComponent(Args&&... args) { data = new T(std::forward<Args>(args)...); }
		virtual ~TComponent() { delete data; }

		virtual void* GetData() const override { return data; }
	};

	//----------Component Manager-----------------//
	class ComponentManager
	{
	public:
		template <typename T, typename... Args>
		T* AddComponent(Entity entt, Args&&... args);

		template <typename T>
		T* GetComponent(Entity entt);
#ifdef SUPPORTED 
		std::vector<void*> GetComponents(Entity entt);
#endif
		template <typename T>
		void RemoveComponent(Entity entt);

		void DestroyEntity(Entity entt);
#define OLD_VIEW 1
#if OLD_VIEW
		template <typename T>
		std::vector<Entity> View();
#endif
		// Function passed in must take a Base* as an argument
		template <typename T, typename Fn>
		void EachComponent(Fn&& func);

	private:
		std::unordered_map<Entity, Component*> m_ComponentMaps[MAX_COMPONENTS];
		size_t m_Size = 0;

	};


	// --------------- Definitions -------------------------------- //

	template <typename T, typename... Args>
	inline T* ComponentManager::AddComponent(Entity entt, Args&&... args)
	{
		ComponentID id = GetComponentID<T>();
		m_Size = std::max(m_Size, (size_t)id + 1);

		TComponent<T>* comp = new TComponent<T>(std::forward<Args>(args)...);
		m_ComponentMaps[id][entt] = comp;
		return comp->data;
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
		return (T*)m_ComponentMaps[id].at(entt)->GetData();
	}

#ifdef SUPPORTED 
	inline std::vector<void*> ComponentManager::GetComponents(Entity entt)
	{
		std::vector<void*> components;
		for (int id = 0; id < m_Size; id++)
		{
			if (m_ComponentMaps[id].find(entt) != m_ComponentMaps[id].end())
				components.push_back(m_ComponentMaps[id].at(entt)->GetData());
		}
		return components;
	}
#endif

	template <typename T>
	inline void ComponentManager::RemoveComponent(Entity entt)
	{
		ComponentID id = GetComponentID<T>();
		delete m_ComponentMaps[id].at(entt);
		m_ComponentMaps[id].erase(entt);
	}

	inline void ComponentManager::DestroyEntity(Entity entt)
	{
		for (ComponentID id = 0; id < m_Size; id++)
		{
			if (m_ComponentMaps[id].find(entt) == m_ComponentMaps[id].end())
				continue;
			delete m_ComponentMaps[id].at(entt);
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

	template <typename T, typename Fn>
	inline void ComponentManager::EachComponent(Fn&& func)
	{
		ComponentID id = GetCompnentID<T>();
		for (auto& [entt, comp] : m_ComponentMaps[id])
		{
			//std::bind(td::forward(func), (ClientComponentType*)comp.get())();
			std::forward<Fn>(func)((T*)comp->GetData());
		}
	}
}