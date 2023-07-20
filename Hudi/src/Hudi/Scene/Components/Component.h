#pragma once

#include "ECS.h"

namespace Hudi {

	class Component
	{
	public:
		Component() = default;
		Component(ECS::Entity _entity, ECS::World* _world) : m_Entity(_entity), world(_world) {}
	public:
		template <typename T, typename... Args>
		T& AddComponent(Args&&... args) { world->AddComponent<T>(m_Entity, args...); return GetComponent<T>(); }

		template <typename T>
		bool HasComponent() { return world->HasComponent<T>(m_Entity); }

		template <typename T>
		T& GetComponent() { return *world->GetComponent<T>(m_Entity); }
		
		void DestroyEntity() { world->DestroyEntity(m_Entity); }

		void SetActive(bool active) { world->SetActive(m_Entity, active); }
		bool IsActive() { return world->IsActive(m_Entity); }

		Component& operator= (const Component& other) { return *this; }

	private:
		ECS::Entity m_Entity = ECS::null;
		ECS::World* world = nullptr;
		friend class GameObject;
	};

}
