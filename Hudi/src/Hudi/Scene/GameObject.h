#pragma once
#include "hdpch.h"

#include "Components/Component.h"

namespace Hudi {

	class Scene;

	class GameObject
	{
	public:
		GameObject();
		GameObject(ECS::World* _world);

		void Destroy();
		void Reset();

		bool Valid() const { return m_Entity > 0 && world != nullptr; }

		ECS::Entity GetEntityID() const { return m_Entity; }

		bool operator== (const GameObject& other);
		bool operator!= (const GameObject& other);

		// ------- System -------/
		bool Exist() { return world->Exists(m_Entity); }

		template <typename T>
		T& AddComponent() { return *world->AddComponent<T>(m_Entity); }

		template <typename T>
		T& AddComponent(Ref<Component> comp) { return *world->AddComponent<T>(m_Entity, comp); }

		template <typename T>
		T& AddComponent(T component) { return *world->AddComponent<T>(m_Entity, NewRef<T>(comp)); }

		template <typename T>
		bool HasComponent() { return world->HasComponent<T>(m_Entity); }

		template <typename T>
		T& GetComponent() { return *world->GetComponent<T>(m_Entity); }

		template <typename T>
		T& GetOrAddComponent() { return HasComponent<T>() ? GetComponent<T>() : AddComponent<T>(); }

		template <typename T>
		Ref<T> GetComponentByRef() { return world->GetComponent<T>(m_Entity); }

		std::vector<Ref<Component>> GetComponents();

		template <typename T>
		void RemoveComponent() { world->RemoveComponent<T>(m_Entity); }

		template <typename T> 
		void AddSystem() { world->AddSystem<T>(m_Entity); }

		void SetActive(bool active) { world->SetActive(m_Entity, active); }
		bool IsActive() { return world->IsActive(m_Entity); }

	private:
		ECS::Entity m_Entity;

	private:
		ECS::World* world;
		friend class Scene;
	};


}

