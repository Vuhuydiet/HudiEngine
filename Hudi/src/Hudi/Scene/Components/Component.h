#pragma once

#include "ECS.h"

namespace Hudi {

	class Component : public ECS::BaseComponent
	{
	public:
		virtual ~Component() = default;
	public:
		template <typename T>
		T& AddComponent() { world->AddComponent<T>(m_Entity, std::make_shared<T>()); return GetComponent<T>(); }

		template <typename T>
		bool HasComponent() { return world->HasComponent<T>(m_Entity); }

		template <typename T>
		T& GetComponent() { return *world->GetComponent<T>(m_Entity); }
		
		template <typename T>
		std::shared_ptr<T> GetComponentByRef() { return world->GetComponent<T>(m_Entity); }

		template <typename T>
		bool Is() { return dynamic_cast<T*>(this); }

		void DestroyEntity() { world->DestroyEntity(m_Entity); }

		void SetActive(bool active) { world->SetActive(m_Entity, active); }
		bool IsActive() { return world->IsActive(m_Entity); }

	public:
		virtual void Awake() {}
		virtual void Update(float dt) {}
	};

}

#include "IDComponent.h"
#include "TransformComponent.h"
#include "SpriteRendererComponent.h"
#include "CameraComponent.h"
#include "Rigidbody2DComponent.h"
#include "Collider2DComponent.h"

#include "BehaviourComponent.h"
#include "AnimatorComponent/AnimationComponent.h"
#include "AnimatorComponent/AnimatorComponent.h"