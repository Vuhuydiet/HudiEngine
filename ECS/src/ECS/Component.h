#pragma once

#include "ecspch.h"
#include "EntityManager.h"

namespace ECS {

	// Base Component struct
	struct Component
	{
		Component()
			: m_Entt(0), enabled(true)
		{}
		virtual ~Component() = default;
		virtual const char* ToString() const = 0;

		virtual void Init() {}

		template <typename T>
		bool Is() { return dynamic_cast<T*>(this); }

		template <typename T>
		T& AddComponent() { Coordinator::AddComponent<T>(m_Entt, std::make_shared<T>()); return GetComponent<T>(); }
			
		template <typename T>
		T& AddComponent(std::shared_ptr<T> comp) { Coordinator::AddComponent<T>(m_Entt, comp); return GetComponent<T>();
		}

		template <typename T>
		T& AddComponent(T component) { Coordinator::AddComponent<T>(m_Entt, std::make_shared<T>(component)); return GetComponent<T>();
		}
		
		template <typename T>
		bool HasComponent() { return Coordinator::HasComponent<T>(m_Entt); }

		template <typename T>
		T& GetComponent() { return Coordinator::GetComponent<T>(m_Entt); }

		virtual const char* OrderOfUpdate() const { return "second"; }
		virtual bool IsAwakable() const { return false; }
		virtual bool IsUpdatable() const { return false; }

		virtual void Awake() {}
		virtual void Update() {}

	public:
		bool enabled = true;
	protected:
		Entity m_Entt;

		friend class Coordinator;
	};

	inline std::ostream& operator<<(std::ostream& os, const Component& e)
	{
		return os << e.ToString();
	}
}