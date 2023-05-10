#pragma once

#include "ECS.h"

namespace Hudi {

	class Component : public ECS::BaseComponent
	{
	public:
		virtual ~Component() = default;
	public:
		template <typename T>
		T& AddComponent() { world->AddComponent<T>(m_Entt, std::make_shared<T>()); return GetComponent<T>(); }
		template <typename T>
		T& AddComponent(std::shared_ptr<T> comp) { world->AddComponent<T>(m_Entt, comp); return GetComponent<T>(); }
		template <typename T>
		T& AddComponent(T component) { world->AddComponent<T>(m_Entt, std::make_shared<T>(component)); return GetComponent<T>(); }

		template <typename T>
		bool HasComponent() { return world->HasComponent<T>(m_Entt); }
		template <typename T>
		bool Is() { return dynamic_cast<T*>(this); }

		template <typename T>
		T& GetComponent() { return *world->GetComponent<T>(m_Entt); }
		template <typename T>
		std::shared_ptr<T> GetComponentByRef() { return world->GetComponent<T>(m_Entt); }

		void DestroyEntity() { world->DestroyEntity(m_Entt); }

		void SetActive(bool active) { world->SetActive(m_Entt, active); }
		bool IsActive() { return world->IsActive(m_Entt); }

	public:
		virtual void Awake() {}
		virtual void Update(float dt) {}
	};

}

#include "TransformComponent.h"
#include "SpriteRendererComponent.h"
#include "RigidBody2DComponent.h"
#include "CameraComponent.h"
#include "BehaviourComponent.h"
#include "AnimatorComponent/AnimationComponent.h"
#include "AnimatorComponent/AnimatorComponent.h"