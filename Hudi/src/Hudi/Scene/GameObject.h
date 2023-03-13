#pragma once
#include "hdpch.h"

#include <ECS.h>

namespace Hudi {

	class GameObject
	{
	public:

		GameObject();
		virtual ~GameObject();

		void CreateGameObject();

		void DestroyGameObject();

		void SetParent(Ref<GameObject> parent);
		void AddChild(Ref<GameObject> child);

		void RemoveChild(Ref<GameObject> child);

		Ref<GameObject>& GetParent();
		std::vector<Ref<GameObject>>& GetChildren();

		operator ECS::Entity() const { return m_Entt; }

		// ------- System -------/
		bool Exist() { return ECS::Coordinator::Exist(m_Entt); }

		template <typename T>
		T& AddComponent() { return ECS::Coordinator::AddComponent<T>(m_Entt, NewRef<T>()); }

		template <typename T>
		T& AddComponent(Ref<ECS::Component> comp) { return ECS::Coordinator::AddComponent<T>(m_Entt, comp); }

		template <typename T>
		T& AddComponent(T component) { return ECS::Coordinator::AddComponent<T>(m_Entt, NewRef<T>(comp)); }

		template <typename T>
		bool HasComponent() { return ECS::Coordinator::HasComponent<T>(m_Entt); }

		template <typename T>
		T& GetComponent() { return ECS::Coordinator::GetComponent<T>(m_Entt); }

		std::vector<Ref<ECS::Component>> GetComponents() { return ECS::Coordinator::GetComponents(m_Entt); }

		template <typename T> 
		void AddSystem() { ECS::Coordinator::AddSystem<T>(m_Entt); }

		void SetActive(bool active) { ECS::Coordinator::SetActive(m_Entt, active); }
		bool IsActive() { return ECS::Coordinator::IsActive(m_Entt); }

	private:
		ECS::Entity m_Entt;

		Ref<GameObject> m_Parent;
		
		std::vector<Ref<GameObject>> m_Children;
	};


}

