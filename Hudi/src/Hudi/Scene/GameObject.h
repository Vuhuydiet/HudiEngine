#pragma once
#include "hdpch.h"

#include "Hudi/Components/Component.h"

namespace Hudi {

	class GameObject
	{
	public:
		GameObject(Ref<ECS::World> _world);
		virtual ~GameObject();

		void CreateGameObject();
		void DestroyGameObject();

		void SetParent(Ref<GameObject> parent);
		void AddChild(Ref<GameObject> child);

		void RemoveParent();
		void RemoveChild(Ref<GameObject> child);

		Ref<GameObject>& GetParent();
		std::vector<Ref<GameObject>>& GetChildren();

		operator ECS::Entity() const { return m_Entt; }
		ECS::Entity GetEntityID() const { return m_Entt; }

		// ------- System -------/
		bool Exist() { return world->Exists(m_Entt); }

		template <typename T>
		T& AddComponent() { return *world->AddComponent<T>(m_Entt); }

		template <typename T>
		T& AddComponent(Ref<Component> comp) { return *world->AddComponent<T>(m_Entt, comp); }

		template <typename T>
		T& AddComponent(T component) { return *world->AddComponent<T>(m_Entt, NewRef<T>(comp)); }

		template <typename T>
		bool HasComponent() { return world->HasComponent<T>(m_Entt); }

		template <typename T>
		T& GetComponent() { return *world->GetComponent<T>(m_Entt); }

		std::vector<Ref<Component>> GetComponents();

		template <typename T> 
		void AddSystem() { world->AddSystem<T>(m_Entt); }

		void SetActive(bool active) { world->SetActive(m_Entt, active); }
		bool IsActive() { return world->IsActive(m_Entt); }

	private:
		ECS::Entity m_Entt;

		Ref<GameObject> m_Parent;
		std::vector<Ref<GameObject>> m_Children;

	private:
		Ref<ECS::World> world;
	};


}

