#pragma once

#include "ECS.h"

namespace Hudi {

#ifdef OLD_ECS
	template <typename T>
	inline bool IsPointerType(void* ptr)
	{
		T* castPtr = dynamic_cast<T*>(static_cast<T*>(ptr));
		return (castPtr != nullptr);
	}

	class Component
	{
	public:
		virtual ~Component() = default;
	public:
		template <typename T, typename... Args>
		T& AddComponent(Args&&... args) { world->AddComponent<T>(m_Entity, args...); return GetComponent<T>(); }

		template <typename T>
		bool HasComponent() { return world->HasComponent<T>(m_Entity); }

		template <typename T>
		T& GetComponent() { return *world->GetComponent<T>(m_Entity); }
		
		template <typename T>
		bool Is() { return IsPointerType<T>(this); }

		void DestroyEntity() { world->DestroyEntity(m_Entity); }

		void SetActive(bool active) { world->SetActive(m_Entity, active); }
		bool IsActive() { return world->IsActive(m_Entity); }

		Component& operator= (const Component& other) { return *this; }

	public:
		virtual void Awake() {}
		virtual void Update(float dt) {}

	private:
		ECS::Entity m_Entity = 0;
		ECS::World* world = nullptr;
		friend class GameObject;
	};
#endif

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

namespace Hudi {

	//template <typename... Args>
	//struct ComponentGroup {};

	//using AllComponents = ComponentGroup<Transform,
	//									IDComponent, 
	//									SpriteRenderer, 
	//									Camera, Rigidbody2D,
	//									BoxCollider2D>;

}