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

		void SetParent(GameObject* parent);
		void AddChild(GameObject* child);

		bool Exist() { return ECS::Coordinator::Exist(m_Entt); }

		template <typename T>
		void AddComponent() { ECS::Coordinator::AddComponent<T>(m_Entt, NewRef<T>()); }

		template <typename T>
		void AddComponent(Ref<ECS::Component> comp) { ECS::Coordinator::AddComponent<T>(m_Entt, comp); }

		template <typename T>
		void AddComponent(T component) { ECS::Coordinator::AddComponent<T>(m_Entt, NewRef<T>(comp)); }

		template <typename T>
		bool HasComponent() { return ECS::Coordinator::HasComponent<T>(m_Entt); }

		template <typename T>
		T& GetComponent() { return ECS::Coordinator::GetComponent<T>(m_Entt); }

		template <typename T> 
		void AddSystem() { ECS::Coordinator::AddSystem<T>(m_Entt); }

		void SetActive(bool active) { ECS::Coordinator::SetActive(m_Entt, active); }
		bool IsActive() { return ECS::Coordinator::IsActive(m_Entt); }

	private:
		ECS::Entity m_Entt;

		GameObject* m_Parent = this;
		
		std::vector<GameObject*> m_Children;
	};


}

